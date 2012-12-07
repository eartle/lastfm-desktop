#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <QPushButton>

class PushButton : public QPushButton
{
    Q_OBJECT
public:
    Q_PROPERTY(bool dark READ dark WRITE setDark)

    explicit PushButton(QWidget *parent = 0);

    bool dark() const;
    void setDark( bool dark );
    
private:
    bool event(QEvent *e);
    void paintEvent(QPaintEvent *);

private:
    bool m_hovered;
    bool m_dark;
};

#endif // PUSHBUTTON_H
