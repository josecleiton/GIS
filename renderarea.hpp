#ifndef RENDERAREA_HPP
#define RENDERAREA_HPP

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QColor>
#include "disk.hpp"
#include "spatialdata.hpp"
using namespace DiskAPI;

class RenderArea : public QWidget
{
    Q_OBJECT
public:
    explicit RenderArea(QWidget *parent = nullptr);
    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;
    void setReg(DiskAPI::Registro*);
protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
signals:

public slots:

private:
    Registro* Reg;
};

#endif // RENDERAREA_HPP
