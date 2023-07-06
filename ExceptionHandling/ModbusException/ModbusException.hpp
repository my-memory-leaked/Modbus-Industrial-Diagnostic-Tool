#pragma once
#include <QException>

class ModbusException : public QException
{
public:
    ModbusException(const QString& cErrorMessage);

    void raise() const override;
    QException *clone() const override;

    const char *what() const noexcept override;

private:
    QString _errorMessage;
};
