/*
 * Copyright (C) 2017 ~ 2018 Wuhan Deepin Technology Co., Ltd.
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

#include <QStandardItemModel>
#include <QScopedPointer>

#include <mediameta.h>

class MusiclistInfomodel : public QStandardItemModel
{
    Q_OBJECT
public:
    MusiclistInfomodel(int rows, int columns, QObject *parent = Q_NULLPTR);
    ~MusiclistInfomodel() override;

//    PlaylistPtr playlist() const;
//    void setPlaylist(PlaylistPtr playlist);

//    MetaPtr meta(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QMap<QString, int> hashIndexs();

    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;
    virtual Qt::DropActions supportedDropActions() const Q_DECL_OVERRIDE;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

};

