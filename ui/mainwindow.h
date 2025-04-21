#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QList>
#include "../json/single_include/nlohmann/json.hpp"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cassert>

class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    QList<QStringList> _models;

private slots:
    void openModelDialog();
    void dumpModels();
    void runMethods();
};

#endif // MAINWINDOW_H
