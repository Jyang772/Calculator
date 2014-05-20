/****************************************************************************
 **
 ** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
 ** Contact: http://www.qt-project.org/legal
 **
 ** This file is part of the examples of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:BSD$
 ** You may use this file under the terms of the BSD license as follows:
 **
 ** "Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are
 ** met:
 **   * Redistributions of source code must retain the above copyright
 **     notice, this list of conditions and the following disclaimer.
 **   * Redistributions in binary form must reproduce the above copyright
 **     notice, this list of conditions and the following disclaimer in
 **     the documentation and/or other materials provided with the
 **     distribution.
 **   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
 **     of its contributors may be used to endorse or promote products derived
 **     from this software without specific prior written permission.
 **
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 ** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 ** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 ** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 ** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 ** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 ** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 ** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 ** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 **
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

/*
 * Rewrite EqualsClicked to parse expression(display)
 * */

 #include <QtGui>
#include <QLineEdit>
#include <QtWidgets>
#include <QDebug>

 #include <math.h>

 #include "button.h"
 #include "calculator.h"

 Calculator::Calculator(QWidget *parent)
     : QWidget(parent)
 {
     sumInMemory = 0.0;
     sumSoFar = 0.0;
     factorSoFar = 0.0;
     waitingForOperand = true;

     display = new QLineEdit("0");
     display->setReadOnly(true);
     display->setAlignment(Qt::AlignRight);
     display->setMaxLength(15);

     QFont font = display->font();
     font.setPointSize(font.pointSize() + 8);
     display->setFont(font);

     for (int i = 0; i < NumDigitButtons; ++i) {
         digitButtons[i] = createButton(QString::number(i), SLOT(digitClicked()));
     }

     Button *pointButton = createButton(tr("."), SLOT(pointClicked()));
     Button *changeSignButton = createButton(tr("\261"), SLOT(changeSignClicked()));

     Button *backspaceButton = createButton(tr("Backspace"), SLOT(backspaceClicked()));
     Button *clearButton = createButton(tr("Clear"), SLOT(clear()));
     Button *clearAllButton = createButton(tr("Clear All"), SLOT(clearAll()));

     Button *clearMemoryButton = createButton(tr("MC"), SLOT(clearMemory()));
     Button *readMemoryButton = createButton(tr("MR"), SLOT(readMemory()));
     Button *setMemoryButton = createButton(tr("MS"), SLOT(setMemory()));
     Button *addToMemoryButton = createButton(tr("M+"), SLOT(addToMemory()));

     Button *divisionButton = createButton(tr("/"), SLOT(multiplicativeOperatorClicked()));
     Button *timesButton = createButton(tr("*"), SLOT(multiplicativeOperatorClicked()));
     Button *minusButton = createButton(tr("-"), SLOT(additiveOperatorClicked()));
     Button *plusButton = createButton(tr("+"), SLOT(additiveOperatorClicked()));

     Button *squareRootButton = createButton(tr("Sqrt"), SLOT(unaryOperatorClicked()));
     Button *powerButton = createButton(tr("x\262"), SLOT(unaryOperatorClicked()));
     Button *reciprocalButton = createButton(tr("1/x"), SLOT(unaryOperatorClicked()));
     Button *equalButton = createButton(tr("="), SLOT(equalClicked()));

     Button *leftParenthesisButton = createButton(tr("("), SLOT(leftParenthesis()));
     Button *rightParenthesisButton = createButton(tr(")"), SLOT(rightParenthesis()));

     QGridLayout *mainLayout = new QGridLayout;
 #if defined(Q_OS_SYMBIAN) || defined(Q_WS_MAEMO_5) || defined(Q_WS_SIMULATOR)
     mainLayout->setSizeConstraint(QLayout::SetNoConstraint);
 #else
     mainLayout->setSizeConstraint(QLayout::SetFixedSize);
 #endif
     mainLayout->addWidget(display, 0, 0, 1, 6);
     mainLayout->addWidget(backspaceButton, 1, 0, 1, 2);
     //mainLayout->addWidget(clearButton, 1, 2, 1, 2);
     //mainLayout->addWidget(clearAllButton, 1, 4, 1, 2);

     mainLayout->addWidget(clearButton, 1, 2, 1, 1);
     mainLayout->addWidget(clearAllButton, 1, 3, 1, 1);

     //Add parenthesis buttons
     mainLayout->addWidget(leftParenthesisButton,1,4,1,1);
     mainLayout->addWidget(rightParenthesisButton,1,5,1,1);
     //

     mainLayout->addWidget(clearMemoryButton, 2, 0);
     mainLayout->addWidget(readMemoryButton, 3, 0);
     mainLayout->addWidget(setMemoryButton, 4, 0);
     mainLayout->addWidget(addToMemoryButton, 5, 0);

     for (int i = 1; i < NumDigitButtons; ++i) {
         int row = ((9 - i) / 3) + 2;
         int column = ((i - 1) % 3) + 1;
         mainLayout->addWidget(digitButtons[i], row, column);
     }

     mainLayout->addWidget(digitButtons[0], 5, 1);
     mainLayout->addWidget(pointButton, 5, 2);
     mainLayout->addWidget(changeSignButton, 5, 3);

     mainLayout->addWidget(divisionButton, 2, 4);
     mainLayout->addWidget(timesButton, 3, 4);
     mainLayout->addWidget(minusButton, 4, 4);
     mainLayout->addWidget(plusButton, 5, 4);

     mainLayout->addWidget(squareRootButton, 2, 5);
     mainLayout->addWidget(powerButton, 3, 5);
     mainLayout->addWidget(reciprocalButton, 4, 5);
     mainLayout->addWidget(equalButton, 5, 5);
     setLayout(mainLayout);

     setWindowTitle(tr("Calculator"));
 }

 void Calculator::digitClicked()
 {
     Button *clickedButton = qobject_cast<Button *>(sender());
     int digitValue = clickedButton->text().toInt();
     if (display->text() == "0" && digitValue == 0.0)
         return;

     if (waitingForOperand) {
         display->clear();
         waitingForOperand = false;
     }
     display->setText(display->text() + QString::number(digitValue));
 }

 void Calculator::unaryOperatorClicked()
 {
     Button *clickedButton = qobject_cast<Button *>(sender());
     QString clickedOperator = clickedButton->text();
     double operand = display->text().toDouble();
     double result = 0.0;

     if (clickedOperator == tr("Sqrt")) {
         if (operand < 0.0) {
             abortOperation();
             return;
         }
         result = sqrt(operand);
     } else if (clickedOperator == tr("x\262")) {
         result = pow(operand, 2.0);
     } else if (clickedOperator == tr("1/x")) {
         if (operand == 0.0) {
             abortOperation();
             return;
         }
         result = 1.0 / operand;
     }
     display->setText(QString::number(result));
     waitingForOperand = true;
 }

 void Calculator::additiveOperatorClicked()
 {


     Button *clickedButton = qobject_cast<Button *>(sender());
     QString clickedOperator = clickedButton->text();

     //Display +
     if(clickedOperator == tr("+"))
     display->setText(display->text() + "+");
     else
         display->setText(display->text() + "-");

 }

 void Calculator::multiplicativeOperatorClicked()
 {
     qDebug() << "-------------------------------" << endl;
     qDebug() << "Multiplicative Operator Clicked" << endl;

     Button *clickedButton = qobject_cast<Button *>(sender());
     QString clickedOperator = clickedButton->text();

     if(clickedOperator == tr("*"))
         display->setText(display->text() + "*");
     else
         display->setText(display->text() + "/");
 }

 void Calculator::equalClicked()
 {

     qDebug() << "Equal Clicked" << endl;

     expr = display->text();
     expr.append(QChar::Null);
     qDebug() << "expr: " << expr << endl;
     int pos = 0;
     value = evalExp(expr, pos);

     display->setText(QString::number(value.toDouble()));

     waitingForOperand = true;
 }

 void Calculator::pointClicked()
 {
     if (waitingForOperand)
         display->setText("0");
     if (!display->text().contains("."))
         display->setText(display->text() + tr("."));
     waitingForOperand = false;
 }

 void Calculator::changeSignClicked()
 {
     QString text = display->text();
     double value = text.toDouble();

     if (value > 0.0) {
         text.prepend(tr("-"));
     } else if (value < 0.0) {
         text.remove(0, 1);
     }
     display->setText(text);
 }

 void Calculator::backspaceClicked()
 {
     if (waitingForOperand)
         return;

     QString text = display->text();
     text.chop(1);
     if (text.isEmpty()) {
         text = "0";
         waitingForOperand = true;
     }
     display->setText(text);
 }

 void Calculator::clear()
 {
     if (waitingForOperand)
         return;

     display->setText("0");
     waitingForOperand = true;
 }

 void Calculator::clearAll()
 {
     sumSoFar = 0.0;
     factorSoFar = 0.0;
     pendingAdditiveOperator.clear();
     pendingMultiplicativeOperator.clear();
     display->setText("0");
     waitingForOperand = true;
 }

 void Calculator::clearMemory()
 {
     sumInMemory = 0.0;
 }

 void Calculator::readMemory()
 {
     display->setText(QString::number(sumInMemory));
     waitingForOperand = true;
 }

 void Calculator::setMemory()
 {
     equalClicked();
     sumInMemory = display->text().toDouble();
 }

 void Calculator::addToMemory()
 {
     equalClicked();
     sumInMemory += display->text().toDouble();
 }
 Button *Calculator::createButton(const QString &text, const char *member)
 {
     Button *button = new Button(text);
     connect(button, SIGNAL(clicked()), this, member);
     return button;
 }

 void Calculator::abortOperation()
 {
     clearAll();
     display->setText(tr("####"));
 }

 bool Calculator::calculate(double rightOperand, const QString &pendingOperator)
 {
     if (pendingOperator == tr("+")) {
         sumSoFar += rightOperand;
     } else if (pendingOperator == tr("-")) {
         sumSoFar -= rightOperand;
     } else if (pendingOperator == tr("\327")) {
         factorSoFar *= rightOperand;
     } else if (pendingOperator == tr("\367")) {
         if (rightOperand == 0.0)
             return false;
         factorSoFar /= rightOperand;
     }
     return true;
 }


//Update Features


 //Parenthesis Functions
 void Calculator::leftParenthesis()
 {
     //Button *clickedButton = qobject_cast<Button *>(sender());
     if(waitingForOperand){
         display->setText("(");
         waitingForOperand = false;
     }
     else
       {
         display->setText(display->text() + "(");
         waitingForOperand = false;
        }
 }

 void Calculator::rightParenthesis()
 {
     //Button *clickedButton = qobject_cast<Button *>(sender());
     display->setText(display->text() + ")");
 }


QVariant Calculator::evalExp(const QString &str, int &pos)
{
    QVariant result = evalTerm(str, pos);
    while(str[pos] != QChar::Null)
    {
        QChar op = str[pos];
        if(op != '+' && op != '-')
            return result;
        ++pos;

        QVariant term = evalTerm(str, pos);
        if(op == '+')
        {
            result = result.toDouble() + term.toDouble();
        }
        else
            result = result.toDouble() - term.toDouble();
    }

    return result;

}

QVariant Calculator::evalTerm(const QString &str, int &pos)
{
    QVariant result = evalFactor(str, pos);
    while(str[pos] != QChar::Null){
        QChar op = str[pos];
        if(op != '*' && op != '/')
            return result;
        ++pos;

        QVariant factor = evalFactor(str, pos);
        if(op == '*'){
            result = result.toDouble() * factor.toDouble();
        }
        else
            result = result.toDouble() / factor.toDouble();
    }

    return result;
}

QVariant Calculator::evalFactor(const QString &str, int &pos)
{
    QVariant result;
    QString token;
    bool negative = false;
    if(str[pos] == '-'){
        negative = true;
        pos++;
    }
    if(str[pos] == '('){
        ++pos;
       result =  evalExp(str, pos);
       qDebug() << result << endl;
        if(str[pos] != ')')
           {
            qDebug() << "ERROR" << endl;
        }
        ++pos;
    }
    else{
        while(str[pos].isNumber() || str[pos] == '.'){
        token += str[pos];
        ++pos;
        }
        result = token.toDouble();

        }

    if(negative){
        result = -str.toDouble();
    }

    return result;
}
