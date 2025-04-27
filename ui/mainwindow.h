#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QList>
#include <QAbstractTableModel>
#include "../json/single_include/nlohmann/json.hpp"
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include "config.h"

class ModelTable : public QAbstractTableModel {
    Q_OBJECT

public:
    explicit ModelTable(QObject *parent = nullptr) : QAbstractTableModel(parent){}
    void setData(const QList<QStringList> &data);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    QList<QStringList> _data;
};

class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    void createDialog(QString title, QString message);
    QList<QStringList> _models;
    ModelTable *model;

private slots:
    void openModelDialog();
    void dumpModels();
    void runMethods();
    void showImage(const QList<QString> &methods);
};

#endif // MAINWINDOW_H
