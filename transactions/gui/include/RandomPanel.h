#ifndef RANDOM_PANEL_H
#define RANDOM_PANEL_H

#include <QWidget>
#include <QSpinBox>

class RandomPanel : public QWidget {
    public:
        RandomPanel(QWidget * parent = nullptr);

        unsigned int ntransactions() const
            {return this->m_transaction_spin->value();}
        unsigned int nvariables() const
            {return this->m_variable_spin->value();}
        unsigned int min_operations() const
            {return this->m_min_operation_spin->value();}
        unsigned int max_operations() const
            {return this->m_max_operation_spin->value();}

    private:
        void init();

        QSpinBox * m_transaction_spin;
        QSpinBox * m_variable_spin;
        QSpinBox * m_min_operation_spin;
        QSpinBox * m_max_operation_spin;
};

#endif // RANDOM_PANEL_H
