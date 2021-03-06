/*
 * Copyright (C) 2016 ~ 2018 Wuhan Deepin Technology Co., Ltd.
 *
 * Author:     Iceyer <me@iceyer.net>
 *
 * Maintainer: Iceyer <me@iceyer.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <QScopedPointer>
#include <DFrame>
#include <DPushButton>
#include <QLabel>

DWIDGET_USE_NAMESPACE

class ImportWidgetPrivate;
class ImportWidget : public DFrame
{
    Q_OBJECT
public:
    explicit ImportWidget(QWidget *parent = Q_NULLPTR);
    ~ImportWidget() override;

    // 向下移动隐藏
    void closeAnimationToDown(const QSize &size);
    // 向左移动显示
    void showAnimationToLeft(const QSize &size);

    const QString getLastImportPath() const;
signals:
    void scanMusicDirectory();
    void selectImportDirectory();
    void importSelectFiles(const QStringList &urllist);

public slots:
    void showWaitHint();
    void showImportHint();
    void slotLinkActivated(const QString &link);
    void slotAddMusicButtonClicked();
    void slotImportPathButtonClicked();
    void slotImportFormDbus(const QUrl &url); //右键菜单打开音乐
    void slotTheme(int type);

protected:
    bool eventFilter(QObject *o, QEvent *e) Q_DECL_OVERRIDE;

protected:
    virtual void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    virtual void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
    virtual void dragLeaveEvent(QDragLeaveEvent *event) Q_DECL_OVERRIDE;
    virtual void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;

private:
    QLabel                  *m_text = nullptr;
    DPushButton             *m_importPathButton = nullptr;
    DPushButton             *m_addMusicButton = nullptr;
    QLabel                  *m_logo = nullptr;
};

