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

 #ifndef CALCULATOR_H
 #define CALCULATOR_H

#include <QWidget>
#include <QVariant>

 class QLineEdit;

 class Button;

 class Calculator : public QWidget
 {
     Q_OBJECT

 public:
     Calculator(QWidget *parent = 0);

 private slots:
     void digitClicked();
     void unaryOperatorClicked();
     void additiveOperatorClicked();
     void multiplicativeOperatorClicked();
     void equalClicked();
     void pointClicked();
     void changeSignClicked();
     void backspaceClicked();
     void clear();
     void clearAll();
     void clearMemory();
     void readMemory();
     void setMemory();
     void addToMemory();

     //Update//
     void leftParenthesis();
     void rightParenthesis();

     QVariant evalExp(const QString &str, int &pos);
     QVariant evalTerm(const QString &str, int &pos);
     QVariant evalFactor(const QString &str, int &pos);



 private:
     Button *createButton(const QString &text, const char *member);
     void abortOperation();
     bool calculate(double rightOperand, const QString &pendingOperator);

     double sumInMemory;
     double sumSoFar;
     double factorSoFar;
     QString pendingAdditiveOperator;
     QString pendingMultiplicativeOperator;
     bool waitingForOperand;

     QLineEdit *display;

     //String for parsing
     QString expr;
     QVariant value;

     enum { NumDigitButtons = 10 };
     Button *digitButtons[NumDigitButtons];
 };

 #endif
