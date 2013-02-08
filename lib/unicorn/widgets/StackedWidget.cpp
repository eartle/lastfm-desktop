#include "StackedWidget.h"

unicorn::StackedWidget::StackedWidget(QWidget *parent) :
    QStackedWidget(parent)
{
    connect( this, SIGNAL(currentChanged(int)), SLOT(onCurrentChanged(int)));
}

QSize
unicorn::StackedWidget::sizeHint() const
{
    return currentWidget()->sizeHint();
}

void
unicorn::StackedWidget::onCurrentChanged( int index )
{
    for ( int i = 0 ; i < count() ; ++i )
    {
        if ( i == index )
        {
            widget( i )->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );
            widget( i )->adjustSize();
        }
        else
            widget( i )->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
    }

    adjustSize();
}
