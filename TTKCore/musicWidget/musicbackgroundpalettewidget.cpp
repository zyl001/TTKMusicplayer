#include "musicbackgroundpalettewidget.h"
#include "ui_musicbackgroundpalettewidget.h"
#include "musicbackgroundmanager.h"
#include "musicuiobject.h"

#include <QDebug>
#include <QGridLayout>
#include <QColorDialog>
#include <QMouseEvent>

#define COLOR_FILE "color.jpg"

MusicBackgroundPalette::MusicBackgroundPalette(QWidget *parent)
    : QLabel(parent)
{
    setCursor(QCursor(Qt::PointingHandCursor));
}

MusicBackgroundPalette::~MusicBackgroundPalette()
{
    QFile::remove(COLOR_FILE);
}

void MusicBackgroundPalette::setPixmap(const QColor &color)
{
    QPixmap pixmap(90, 30);
    pixmap.fill( m_color = color );
    QLabel::setPixmap(pixmap);
}

void MusicBackgroundPalette::copyColorToMemory(const QColor &color)
{
    QImage image(16, 16, QImage::Format_ARGB32);
    image.fill(color);
    if(image.save( COLOR_FILE ))
    {
        emit currentColorToMemoryChanged( COLOR_FILE );
    }
}

void MusicBackgroundPalette::enterEvent(QEvent *event)
{
    QLabel::enterEvent(event);
    copyColorToMemory(m_color);
}

void MusicBackgroundPalette::mousePressEvent(QMouseEvent *event)
{
//    QLabel::mousePressEvent(event);
    if(event->button() == Qt::LeftButton)
    {
        currentColorToFileChanged(m_color);
    }
}


MusicBackgroundPaletteWidget::MusicBackgroundPaletteWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicBackgroundPaletteWidget)
{
    ui->setupUi(this);
    m_confirmButtonClicked = false;

    QList<QColor> colors;
    colors << QColor(225, 152, 180);
    colors << QColor(236, 109, 113);
    colors << QColor(208, 87, 107);
    colors << QColor(233, 84, 107);
    colors << QColor(197, 61, 67);
    colors << QColor(232, 57, 41);
    colors << QColor(226, 4, 27);
    colors << QColor(201, 23, 30);
    colors << QColor(158, 61, 63);
    colors << QColor(162, 32, 65);

    colors << QColor(251, 202, 77);
    colors << QColor(250, 191, 20);
    colors << QColor(230, 180, 34);
    colors << QColor(217, 166, 46);
    colors << QColor(243, 152, 0);
    colors << QColor(236, 104, 0);
    colors << QColor(234, 85, 6);
    colors << QColor(191, 120, 58);
    colors << QColor(119, 60, 48);
    colors << QColor(100, 1, 37);

    colors << QColor(204, 126, 177);
    colors << QColor(180, 76, 151);
    colors << QColor(157, 91, 139);
    colors << QColor(122, 65, 113);
    colors << QColor(136, 72, 152);
    colors << QColor(116, 50, 92);
    colors << QColor(112, 88, 163);
    colors << QColor(77, 67, 152);
    colors << QColor(77, 90, 175);
    colors << QColor(46, 41, 48);

    colors << QColor(160, 216, 239);
    colors << QColor(132, 185, 203);
    colors << QColor(0, 163, 175);
    colors << QColor(105, 185, 198);
    colors << QColor(0, 149, 217);
    colors << QColor(39, 146, 195);
    colors << QColor(76, 108, 179);
    colors << QColor(62, 98, 173);
    colors << QColor(25, 68, 142);
    colors << QColor(42, 64, 115);

    colors << QColor(195, 216, 37);
    colors << QColor(184, 210, 0);
    colors << QColor(199, 220, 104);
    colors << QColor(147, 202, 118);
    colors << QColor(131, 155, 92);
    colors << QColor(105, 176, 118);
    colors << QColor(71, 136, 94);
    colors << QColor(2, 135, 96);
    colors << QColor(71, 89, 80);
    colors << QColor(0, 82, 67);

    colors << QColor(153, 153, 153);
    colors << QColor(165, 143, 134);
    colors << QColor(159, 111, 85);
    colors << QColor(118, 92, 71);
    colors << QColor(84, 47, 50);
    colors << QColor(102, 102, 102);
    colors << QColor(83, 78, 77);
    colors << QColor(43, 43, 43);
    colors << QColor(37, 13, 0);
    colors << QColor(22, 22, 14);
    /////////////////////////////////////////
    QGridLayout *layout = new QGridLayout(ui->mutliWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    for(int i=0; i<6; ++i)
        for(int j=0; j<10; ++j)
        {
            MusicBackgroundPalette *label = new MusicBackgroundPalette(this);
            label->setPixmap( colors[i*10 + j] );
            m_widgets << label;
            layout->addWidget(label, i, j);
            connect(label, SIGNAL(currentColorToFileChanged(QColor)),
                           SLOT(currentColorToFile(QColor)));
            connect(label, SIGNAL(currentColorToMemoryChanged(QString)),
                           SLOT(currentColorToMemory(QString)));
        }
    ui->mutliWidget->setLayout(layout);

    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));

    ui->paletteButton->setStyleSheet(MusicUIObject::MPushButtonStyle08);
    ui->confirmButton->setStyleSheet(MusicUIObject::MPushButtonStyle08);

    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));
    connect(ui->paletteButton, SIGNAL(clicked()), SLOT(showPaletteDialog()));
    connect(ui->confirmButton, SIGNAL(clicked()), SLOT(paletteColorClicked()));
}

MusicBackgroundPaletteWidget::~MusicBackgroundPaletteWidget()
{
    QFile::remove(COLOR_FILE);
    if(!m_confirmButtonClicked)
    {
        emit currentColorToMemoryChanged( M_BACKGROUND_PTR->getMBackground() );
    }
    while(!m_widgets.isEmpty())
    {
        delete m_widgets.takeLast();
    }
    delete ui;
}

void MusicBackgroundPaletteWidget::updateBackground(const QString &text)
{
    QPixmap pix(text);
    ui->background->setPixmap(pix.scaled( size() ));
}

void MusicBackgroundPaletteWidget::paletteColorClicked()
{
    if(m_currentColor.isValid())
    {
        m_confirmButtonClicked = true;
        QImage image(16, 16, QImage::Format_ARGB32);
        image.fill(m_currentColor);
        if(image.save( COLOR_FILE ))
        {
            emit currentColorToFileChanged( COLOR_FILE );
        }
    }
    close();
}

void MusicBackgroundPaletteWidget::showPaletteDialog()
{
    QColor paletteColor = QColorDialog::getColor(Qt::white, this);
    if(!paletteColor.isValid())
    {
        return;
    }
    currentColorToFile( m_currentColor = paletteColor );
}

void MusicBackgroundPaletteWidget::currentColorToFile(const QColor &color)
{
    QPixmap pixmap(90, 30);
    pixmap.fill(color);
    ui->colorLabel->setPixmap(pixmap);

    QImage image(16, 16, QImage::Format_ARGB32);
    image.fill(m_currentColor = color);
    if(image.save( COLOR_FILE ))
    {
        currentColorToMemory( COLOR_FILE );
    }
}

void MusicBackgroundPaletteWidget::currentColorToMemory(const QString &path)
{
    updateBackground(path);
    emit currentColorToMemoryChanged( path );
}

int MusicBackgroundPaletteWidget::exec()
{
    updateBackground(M_BACKGROUND_PTR->getMBackground());
    return MusicAbstractMoveDialog::exec();
}
