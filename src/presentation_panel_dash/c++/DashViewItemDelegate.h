/*
 * Copyright (C) 2014 Moonlight Desktop Environment Team
 * Authors:
 *      Jorge Alberto Díaz Orozco
 * This file is part of Moonlight Desktop Environment.
 *
 * Moonlight Desktop Environment is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Moonlight Desktop Environment is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Moonlight Desktop Environment. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef DASHVIEWITEMDELEGATE_H
#define	DASHVIEWITEMDELEGATE_H

#include <QStyledItemDelegate>

class DashViewItemDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    DashViewItemDelegate();
    virtual ~DashViewItemDelegate();
    void paint(QPainter *, const QStyleOptionViewItem &, const QModelIndex &) const ;
    QSize sizeHint(const QStyleOptionViewItem & , const QModelIndex &) const ;
private:

};

#endif	/* DASHVIEWITEMDELEGATE_H */
