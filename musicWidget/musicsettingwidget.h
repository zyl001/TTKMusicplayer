#ifndef MUSICSETTINGWIDGET_H
#define MUSICSETTINGWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovedialog.h"
#include "musicuiobject.h"
#include "musicsettingmanager.h"

class QComboBox;

namespace Ui {
class MusicSettingWidget;
}

class MUSIC_WIDGET_EXPORT MusicSettingWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    enum Type
    {
        Inline,
        Desktop
    };

    explicit MusicSettingWidget(QWidget *parent = 0);
    virtual ~MusicSettingWidget();

    void initControllerParameter();

Q_SIGNALS:
    void parameterSettingChanged();

public Q_SLOTS:
    void downloadGroupCached(int index);
    void downloadGroupSpeedLimit(int index);
    void downloadDirSelected(int index);
    void changeInlineLrcWidget();
    void changeDesktopLrcWidget();
    void commitTheResults();

    void inlineLrcFgChanged();
    void inlineLrcBgChanged();
    void defaultLrcColorChanged(int index);
    void inlineLrcTransChanged(int index);
    void showInlineLrcDemo(int index);
    void resetInlineParameter();

    void desktopFgChanged();
    void desktopBgChanged();
    void defaultDesktopLrcColorChanged(int index);
    void desktopLrcTransChanged(int index);
    void showDesktopLrcDemo(int index);
    void resetDesktopParameter();
    virtual int exec();

protected:
    void initInlineLrcWidget();
    void initDesktopLrcWidget();
    void initDownloadWidget();
    void lcrColorValue(Type key, QString type, QPushButton *obj);
    void lrcColorByDefault(Type key, int index);
    void lrcTransparentValue(Type key, int index) const;
    void setComboboxText(QComboBox *object, const QString &text);

    Ui::MusicSettingWidget *ui;
    QColor m_lrcSelectedFg;
    QColor m_lrcSelectedBg;
    QColor m_DlrcSelectedFg;
    QColor m_DlrcSelectedBg;

};

#endif // MUSICSETTINGWIDGET_H