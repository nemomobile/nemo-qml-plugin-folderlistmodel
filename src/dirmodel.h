/*
 * Copyright (C) 2012 Robin Burchell <robin+nemo@viroteck.net>
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * "Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of Nemo Mobile nor the names of its contributors
 *     may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 */

#ifndef DIRMODEL_H
#define DIRMODEL_H

#include <QAbstractListModel>
#include <QFileInfo>
#include <QVector>
#include <QStringList>
#include <QDir>
#include <QDateTime>

#include "iorequest.h"
#include "filecompare.h"

class FileSystemAction;

/*!
 *  When the External File System Wathcer is enabled,
 *  this is the interval used to check if there has been any change in the current path
 *
 *  \sa setEnabledExternalFSWatcher()
 */
#define EX_FS_WATCHER_TIMER_INTERVAL   3100

class DirModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        FileNameRole = Qt::UserRole,
        CreationDateRole,
        ModifiedDateRole,
        FileSizeRole,
        IconSourceRole,
        FilePathRole,
        IsDirRole,
        IsFileRole,
        IsReadableRole,
        IsWritableRole,
        IsExecutableRole,
        TrackTitleRole,
        TrackArtistRole,
        TrackAlbumRole,
        TrackYearRole,
        TrackNumberRole,
        TrackGenreRole,
        TrackLengthRole,
        TrackCoverRole
    };

public:
    explicit DirModel(QObject *parent = 0);
    ~DirModel();

    int rowCount(const QModelIndex &index = QModelIndex()) const
    {
        if (index.parent() != QModelIndex())
            return 0;
        return mDirectoryContents.count();
    }

    // TODO: this won't be safe if the model can change under the holder of the row
    Q_INVOKABLE QVariant data(int row, const QByteArray &stringRole) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    Q_INVOKABLE void refresh()
    {
        // just some syntactical sugar really
        setPath(path());
    }

    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
    inline QString path() const { return mCurrentDir; }
    void setPath(const QString &pathName);

    Q_PROPERTY(QDateTime pathCreatedDate  READ pathCreatedDate)
    QDateTime pathCreatedDate() const;

    Q_PROPERTY(QDateTime pathModifiedDate READ  pathModifiedDate)
    QDateTime   pathModifiedDate() const;

    Q_PROPERTY(QString pathCreatedDateLocaleShort  READ  pathCreatedDateLocaleShort)
    QString     pathCreatedDateLocaleShort() const;

    Q_PROPERTY(QString  pathModifiedDateLocaleShort READ pathModifiedDateLocaleShort)
    QString     pathModifiedDateLocaleShort() const;

    Q_PROPERTY(bool pathIsWritable  READ pathIsWritable)
    bool pathIsWritable() const;

    Q_PROPERTY(bool awaitingResults READ awaitingResults NOTIFY awaitingResultsChanged)
    bool awaitingResults() const;

    Q_INVOKABLE void rm(const QStringList &paths);

    Q_INVOKABLE bool rename(int row, const QString &newName);

    Q_INVOKABLE void mkdir(const QString &newdir);

    Q_PROPERTY(bool filterDirectories READ filterDirectories WRITE setFilterDirectories NOTIFY filterDirectoriesChanged)
    bool filterDirectories() const;

    Q_PROPERTY(bool isRecursive READ isRecursive WRITE setIsRecursive NOTIFY isRecursiveChanged)
    bool isRecursive() const;

    Q_PROPERTY(bool readsMediaMetadata READ readsMediaMetadata WRITE setReadsMediaMetadata NOTIFY readsMediaMetadataChanged)
    bool readsMediaMetadata() const;

    Q_PROPERTY(bool showDirectories READ showDirectories WRITE setShowDirectories NOTIFY showDirectoriesChanged)
    bool showDirectories() const;

    Q_PROPERTY(QStringList nameFilters READ nameFilters WRITE setNameFilters NOTIFY nameFiltersChanged)
    QStringList nameFilters() const;
    void setNameFilters(const QStringList &nameFilters);

public slots:
    void onItemsAdded(const QVector<QFileInfo> &newFiles);
    void onResultsFetched();

signals:
    void awaitingResultsChanged();
    void nameFiltersChanged();
    void filterDirectoriesChanged();
    void isRecursiveChanged();
    void readsMediaMetadataChanged();
    void showDirectoriesChanged();
    void pathChanged(const QString& newPath);
    void error(const QString &errorTitle, const QString &errorMessage);

private:
    QHash<int, QByteArray> buildRoleNames() const;
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    // In Qt5, the roleNames() is virtual and will work just fine. On qt4 setRoleNames must be used with buildRoleNames.
    QHash<int, QByteArray> roleNames() const;
#endif

    QStringList mNameFilters;
    bool mFilterDirectories;
    bool mShowDirectories;
    bool mAwaitingResults;
    bool mIsRecursive;
    bool mReadsMediaMetadata;
    QString mCurrentDir;
    QVector<QFileInfo> mDirectoryContents;

public:
    //[0] new stuff Ubuntu File Manager
#if defined(REGRESSION_TEST_FOLDERLISTMODEL)
    //make this work with tables
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const
    {
        Q_UNUSED(parent);
        return IsExecutableRole - FileNameRole + 1;
    }
    virtual QVariant  headerData(int section, Qt::Orientation orientation, int role) const;
#endif

    Q_PROPERTY(QString parentPath READ parentPath NOTIFY pathChanged)
    QString parentPath() const;

    Q_PROPERTY(bool showHiddenFiles READ getShowHiddenFiles WRITE setShowHiddenFiles NOTIFY showHiddenFilesChanged)
    bool getShowHiddenFiles() const;

    Q_ENUMS(SortBy)
    enum SortBy
    {
        SortByName,
        SortByDate
    };
    Q_PROPERTY(SortBy sortBy READ getSortBy WRITE setSortBy NOTIFY sortByChanged)
    SortBy getSortBy() const;

    Q_ENUMS(SortOrder)
    enum SortOrder
    {
        SortAscending   = Qt::AscendingOrder,
        SortDescending = Qt::DescendingOrder
    };
    Q_PROPERTY(SortOrder sortOrder READ getSortOrder WRITE setSortOrder NOTIFY sortOrderChanged)
    SortOrder getSortOrder() const;

    Q_PROPERTY(int clipboardUrlsCounter READ getClipboardUrlsCounter NOTIFY clipboardChanged)
    int getClipboardUrlsCounter() const;

    Q_PROPERTY(bool enableExternalFSWatcher READ getEnabledExternalFSWatcher WRITE setEnabledExternalFSWatcher)
    bool  getEnabledExternalFSWatcher() const;

    Q_INVOKABLE QString homePath() const;

    /*!
     *    \brief Tries to make the directory pointed by row as the current to be browsed
     *    \return true if row points to a directory and the directory is readble, false otherwise
     */
    Q_INVOKABLE  bool cdIntoIndex(int row);

    Q_INVOKABLE  bool cdIntoPath(const QString& filename);
    /*!
     * \brief copyIndex() puts the item pointed by \a row (dir or file) into the clipboard
     * \param row points to the item file or directory
     */
    Q_INVOKABLE void  copyIndex(int row);

    /*!
     *  \brief copyPaths(const QStringList& urls) several items (dirs or files) into the clipboard
     *  \param items  fullpathnames or names only
     */
    Q_INVOKABLE void  copyPaths(const QStringList& items);

    /*!
     * \brief cutIndex() puts the item into the clipboard as \ref copy(),
     *        mark the item to be removed after \ref paste()
     * \param row points to the item file or directory
     */
    Q_INVOKABLE void  cutIndex(int row);

    /*!
     *  \brief cut() puts several items (dirs or files) into the clipboard as \ref copy(),
     *         mark the item to be removed after \ref paste()
     *   \param items  fullpathnames or names only
     */
    Q_INVOKABLE void  cutPaths(const QStringList& items);

    /*!
     * \brief removeIndex();  remove a item file or directory
     *
     * I gets the item indicated by \row and calls \ref rm()
     *
     * \param row points to the item to b e removed
     * \return true if it was possible to remove the item
     */
    Q_INVOKABLE void removeIndex(int row);

    /*!
     *  Just calls \ref rm()
     */
    Q_INVOKABLE void removePaths(const QStringList& items);

    /*!
     *  Tries to open a file using a suitable application, if the index points to a directory
     *  it goes into it using \ref cdIntoIndex() or \ref cdIntoPath()
     *
     *  \note Qt uses Qt QDesktopServices::openUrl()
     */
    Q_INVOKABLE bool  openIndex(int row);

    /*!
     *  Same as \ref openIndex() but using a file name instead of index
     *
     *  It allows to open directories and files using absoulte paths
     *
     *  \sa \ref cdIntoPath()
     */
    Q_INVOKABLE bool  openPath(const QString& filename);

    // some helper functions that can be useful to other QML applications than File Manager
    Q_INVOKABLE  bool  existsDir(const QString&  folderName) const;
    Q_INVOKABLE  bool  canReadDir(const QString& folderName) const;
    Q_INVOKABLE  bool  existsFile(const QString& fileName)   const;
    Q_INVOKABLE  bool  canReadFile(const QString& fileName)  const;

public slots:
    /*!
     * \brief goHome() goes to user home dir
     *  Go to user home dir, we may have a tab for places or something like that
     */
    void  goHome();

    /*!
     * \brief cdUp() sets the parent directory as current directory
     *
     *  It can work as a back function if there is no user input path
     * \return true if it was possible to change to parent dir, otherwise false
     */
    bool  cdUp();

    /*!
     * \brief paste() copy item(s) from \ref copy() and \ref paste() into the current directory
     *
     *  If the operation was \ref cut(), then remove the original item
     */
    void paste();

    /*!
     * \brief cancelAction() any copy/cut/remove can be cancelled
     */
    void cancelAction();    

    void setIsRecursive(bool isRecursive);
    void setReadsMediaMetadata(bool readsMediaMetadata);
    void setFilterDirectories(bool filterDirectories);
    void setShowDirectories(bool showDirectories);
    void setShowHiddenFiles(bool show);
    void setSortBy(SortBy field);
    void setSortOrder(SortOrder order);
    void setEnabledExternalFSWatcher(bool enable);


    void toggleShowDirectories();
    void toggleShowHiddenFiles();
    void toggleSortOrder();
    void toggleSortBy();

signals:
    /*!
     * \brief insertedItem()
     *
     *  It happens when a new file is inserted in an existent view,
     *  for example from  \ref mkdir() or \ref paste()
     *
     *  It can be used to make the new row visible to the user doing a scroll to
     */
    void  insertedRow(int row);
    /*!
     * \brief progress()
     *  Sends status about recursive and multi-items remove/move/copy
     *
     * \param curItem     current item being handled
     * \param totalItems  total of items including recursive directories content
     * \param percent     a percent done
     */
    void     progress(int curItem, int totalItems, int percent);

    void     showHiddenFilesChanged();
    void     sortByChanged();
    void     sortOrderChanged();

    void     clipboardChanged();

private slots:
    void onItemRemoved(const QString&);
    void onItemRemoved(const QFileInfo&);
    void onItemAdded(const QString&);
    void onItemAdded(const QFileInfo&);

private:
    int           addItem(const QFileInfo& fi);
    void          setCompareAndReorder();
    int           rowOfItem(const QFileInfo& fi);
    QDir::Filter  currentDirFilter()  const;
    QString       dirItems(const QFileInfo& fi) const;
    bool          cdInto(const QFileInfo& fi);
    bool          openItem(const QFileInfo& fi);
    DirListWorker * createWorkerRequest(IORequest::RequestType requestType,
                                                  const QString& pathName);
    bool          canReadDir(const QFileInfo& d)   const;
    bool          canReadFile(const QFileInfo& f)  const;
    QFileInfo     setParentIfRelative(const QString &fileOrDir) const;

private:
    void          startExternalFsWatcher();
    void          stoptExternalFsWatcher();
private slots:
    void          onFetchingContents(QDateTime);
    void          onItemAddedOutsideFm(const QFileInfo&fi);
    void          onItemRemovedOutSideFm(const QFileInfo&);
    void          onItemChangedOutSideFm(const QFileInfo&fi);
    void          onExternalFsWatcherFinihed();
protected:
   virtual void   timerEvent(QTimerEvent *);

private:
    bool               mShowHiddenFiles;
    SortBy             mSortBy;
    SortOrder          mSortOrder;
    CompareFunction    mCompareFunction;
    int                mExternalFSWatcherTimer;
    bool               mEnableExternalFSWatcher;
    QDateTime          mLastModifiedCurrentPath;

#if defined(REGRESSION_TEST_FOLDERLISTMODEL) //used in Unit/Regression tests
public:
#else
private:
#endif
    FileSystemAction  *  m_fsAction;  //!< it does file system recursive remove/copy/move
    QString fileSize(qint64 size)  const;
//[0]
};


#endif // DIRMODEL_H
