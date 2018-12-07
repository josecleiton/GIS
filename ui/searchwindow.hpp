#ifndef SEARCHWINDOW_HPP
#define SEARCHWINDOW_HPP

#include <sstream>
#include <QDialog>
#include "icon.hpp"
#include "rectanglesearchwindow.hpp"
#include "findwindow.hpp"
#include "insertpointwindow.hpp"
#include "variaveis.hpp"
#include "stringwindow.hpp"
#include "rtree.hpp"

namespace Ui {
class SearchWindow;
}

class SearchWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SearchWindow(QWidget *parent = nullptr);
    ~SearchWindow();
    bool getNuke();

private slots:
    void on_retangulo_clicked();

    void on_interseccao_clicked();

    void on_ponto_clicked();

    void on_id_clicked();

    void on_all_clicked();

private:
    Ui::SearchWindow *ui;
    bool nuke{};
};

#endif // SEARCHWINDOW_HPP
