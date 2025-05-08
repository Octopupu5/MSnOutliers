#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QList>
#include <QAbstractTableModel>
#include <QPushButton>
#include <QTableView>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QDebug>
#include <QFile>
#include <QLabel>

#include "../json/single_include/nlohmann/json.hpp"
#include "generatedialog.h"
#include "modeldialog.h"

#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include "config.h"

namespace CP {
    namespace UI {
        void createDialog(QString title, QString message);

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
            QList<QStringList> _models;
            ModelTable *model;

        private slots:
            void openModelDialog();
            void dumpModels();
            void runMethods();
            void generateData();
            void showImage(const QList<QString> &methods);
        };
    } // namespace UI;
} // namespace CP;

#endif // MAINWINDOW_H
