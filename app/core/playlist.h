/**
 * Copyright (C) 2016 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QObject>

#include <QSettings>

#include "../model/musiclistmodel.h"

class Playlist : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ displayName WRITE setDisplayName NOTIFY displayNameChanged)
public:
    explicit Playlist(const MusicListInfo &musiclistinfo, QObject *parent = 0);

public:
    //! public interface
    QString id();
    QString displayName();
    QString icon();
    bool readonly();
    bool editmode();
    int length();

    bool contains(const MusicInfo &info);
    MusicList allmusic();

public slots:
    void setDisplayName(const QString &name);
    void appendMusic(const MusicInfo &listinfo);
    void removeMusic(const MusicInfo &listinfo);

    //! private interface
public:
    void load();
    void save();

signals:
    void removed();
    void displayNameChanged(QString displayName);

private:
    QSettings       settings;
    MusicListInfo   listinfo;
};

#endif // PLAYLIST_H