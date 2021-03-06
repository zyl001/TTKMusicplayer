#include "musicmydownloadrecordwidget.h"
#include "musicmessagebox.h"
#include "musicconnectionpool.h"
#include "musicitemdelegate.h"

#include <QMenu>
#include <QContextMenuEvent>

MusicMyDownloadRecordWidget::MusicMyDownloadRecordWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    setColumnCount(4);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 10);
    headerview->resizeSection(1, 170);
    headerview->resizeSection(2, 83);
    headerview->resizeSection(3, 50);

    m_delegate = new MusicProgressBarDelegate(this);
    setItemDelegateForColumn(2, m_delegate);
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    connect(this, SIGNAL(cellDoubleClicked(int,int)), SLOT(listCellDoubleClicked(int,int)));
    musicSongsFileName();

    M_CONNECTION_PTR->setValue("MusicMyDownloadRecordWidget", this);
    M_CONNECTION_PTR->poolConnect("MusicMyDownloadRecordWidget", "MusicSongsSummarizied");
}

MusicMyDownloadRecordWidget::~MusicMyDownloadRecordWidget()
{
    M_CONNECTION_PTR->poolDisConnect("MusicMyDownloadRecordWidget");
    delete m_delegate;
    clearAllItems();
    MusicMyDownloadRecordConfigManager xml;
    xml.writeDownloadConfig(m_musicRecord);
}

void MusicMyDownloadRecordWidget::musicSongsFileName()
{
    MusicMyDownloadRecordConfigManager xml;
    if(!xml.readDownloadXMLConfig())
    {
        return;
    }
    xml.readDownloadConfig(m_musicRecord);

    setRowCount(m_loadRecordCount = m_musicRecord.m_names.count()); //reset row count
    for(int i=0; i<m_musicRecord.m_names.count(); i++)
    {
        createItem(i, m_musicRecord.m_names[i], m_musicRecord.m_sizes[i], 999);
    }
}

void MusicMyDownloadRecordWidget::createItem(int index, const QString &name,
                                             const QString &size, qint64 time)
{
    QTableWidgetItem *item = new QTableWidgetItem;
    setItem(index, 0, item);

                      item = new QTableWidgetItem;
    item->setText(QFontMetrics(font()).elidedText(name, Qt::ElideRight, 160));
    item->setTextColor(QColor(50, 50, 50));
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    item->setToolTip( name );
    setItem(index, 1, item);

                      item = new QTableWidgetItem;
    item->setData(MUSIC_PROCS_ROLE, 100);
    setItem(index, 2, item);

                      item = new QTableWidgetItem( size );
    item->setTextAlignment(Qt::AlignCenter);
    item->setData(MUSIC_TIMES_ROLE, time);
    setItem(index, 3, item);
}

void MusicMyDownloadRecordWidget::clearAllItems()
{
    //Remove all the original item
    MusicAbstractTableWidget::clear();
    setColumnCount(4);
}

void MusicMyDownloadRecordWidget::contextMenuEvent(QContextMenuEvent *event)
{
    MusicAbstractTableWidget::contextMenuEvent(event);
    QMenu rightClickMenu(this);
    rightClickMenu.setStyleSheet(MusicUIObject::MMenuStyle02);
    rightClickMenu.addAction(QIcon(":/contextMenu/play"), tr("musicPlay"), this, SLOT(musicPlay()));
    rightClickMenu.addAction(tr("openFileDir"), this, SLOT(musicOpenFileDir()));
    rightClickMenu.addSeparator();
    rightClickMenu.addAction(QIcon(":/contextMenu/delete"), tr("delete"), this, SLOT(setDeleteItemAt()));
    rightClickMenu.addAction(tr("deleteAll"), this, SLOT(setDeleteItemAll()));
    rightClickMenu.exec(QCursor::pos());
    event->accept();
}

void MusicMyDownloadRecordWidget::setDeleteItemAll()
{
    selectAll();
    setDeleteItemAt();
}

void MusicMyDownloadRecordWidget::setDeleteItemAt()
{
    MusicMessageBox message;
    message.setText(tr("Are you sure to delete?"));
    if( message.exec() || rowCount() == 0 )
    {
       return;
    }

    MusicObject::MIntSet deletedRow; //if selected multi rows
    foreach(QTableWidgetItem *item, selectedItems())
    {
        deletedRow.insert(item->row());
    }

    MusicObject::MIntList deleteList = deletedRow.toList();
    qSort(deleteList);
    for(int i=deleteList.count() - 1; i>=0; --i)
    {
        int index = deleteList[i];
        removeRow(index); //Delete the current row
        m_musicRecord.m_names.removeAt(index);
        m_musicRecord.m_paths.removeAt(index);
        m_musicRecord.m_sizes.removeAt(index);
        --m_loadRecordCount;
    }
}

void MusicMyDownloadRecordWidget::listCellClicked(int, int)
{

}

void MusicMyDownloadRecordWidget::listCellDoubleClicked(int, int)
{
    musicPlay();
}

void MusicMyDownloadRecordWidget::musicOpenFileDir()
{
    if(rowCount() == 0 || currentRow() < 0)
    {
        return;
    }

    if(!MusicUtils::openUrl(QFileInfo(m_musicRecord.m_paths[currentRow()]).absoluteFilePath(), true))
    {
        MusicMessageBox message;
        message.setText(tr("The origin one does not exist!"));
        message.exec();
    }
}

void MusicMyDownloadRecordWidget::musicPlay()
{
    if(rowCount() == 0 || currentRow() <0)
    {
        return;
    }
    emit addSongToPlay(QStringList(m_musicRecord.m_paths[currentRow()]));
}

void MusicMyDownloadRecordWidget::downloadProgressChanged(float percent, const QString &total, qint64 time)
{
    for(int i=m_loadRecordCount; i<rowCount(); ++i)
    {
        QTableWidgetItem *it = item(i, 3);
        if(it && it->data(MUSIC_TIMES_ROLE).toLongLong() == time)
        {
            item(i, 2)->setData(MUSIC_PROCS_ROLE, percent);
            item(i, 3)->setText( total );
            if(percent == 100)
            {
                m_musicRecord.m_sizes[i] = total;
            }
            break;
        }
    }
}

void MusicMyDownloadRecordWidget::createDownloadItem(const QString &name, qint64 time)
{
    setRowCount( rowCount()  + 1);
    QString musicName = name;
    musicName.remove(MUSIC_DIR_FULL).chop(4);

    m_musicRecord.m_names << musicName;
    m_musicRecord.m_paths << QFileInfo(name).absoluteFilePath();
    m_musicRecord.m_sizes << "0.0M";

    createItem(rowCount() - 1, musicName, "0.0M", time);
}
