#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "calc.h"

enum {
  STRING_CAPACITY = 256
};

GtkWidget * window;
GtkWidget * gridBasic;
GtkWidget * entryDisplay;
GtkWidget * btnMemoryClear;
GtkWidget * btnMemoryRecall;
GtkWidget * btnMemoryPlus;
GtkWidget * btnMemoryMinus;
GtkWidget * btnPercent;
GtkWidget * btnSquareRoot;
GtkWidget * btnSquare;
GtkWidget * btnReciprocal;
GtkWidget * btnClearEntry;
GtkWidget * btnAllClear;
GtkWidget * btnBackspace;
GtkWidget * btnPlus;
GtkWidget * btnMinus;
GtkWidget * btnTimes;
GtkWidget * btnDivide;
GtkWidget * btnEqual;
GtkWidget * btnPositiveNegative;
GtkWidget * btnDot;
GtkWidget * btnDigit[10];

char operand1[STRING_CAPACITY] = {'\0'};
char binaryOperator[2] = {'\0'};
char operand2[STRING_CAPACITY] = {'\0'};
char result[STRING_CAPACITY] = {'\0'};
char memory[STRING_CAPACITY] = {'\0'};
bool flag = false;

void strclr(char * str) {
  sprintf(str, "%s", "\0");  
}

void on_btnAllClear_clicked(void) {
  gtk_entry_set_text(GTK_ENTRY(entryDisplay), "0");
  strclr(operand1);
  strclr(binaryOperator);
  strclr(operand2);
  strclr(result);
  flag = false;
}

void on_btnDigit_clicked(GtkWidget * btn) {
  if (strlen(result) != 0) {
    on_btnAllClear_clicked();
  }
  if (flag) {
    gtk_entry_set_text(GTK_ENTRY(entryDisplay), "0");
    flag = false;
  }
  const char * numberDisplayed = gtk_entry_get_text(GTK_ENTRY(entryDisplay));
  const char * digit = gtk_button_get_label(GTK_BUTTON(btn));
  if (isdigit(*digit) && strcmp(numberDisplayed, "0") == 0) {
    gtk_entry_set_text(GTK_ENTRY(entryDisplay), "");
  }
  if (!(*digit == '.' && strchr(numberDisplayed, '.'))) {
    char numberDisplayedWithDot[STRING_CAPACITY] = {'\0'};
    strcpy(numberDisplayedWithDot, numberDisplayed);
    strcat(numberDisplayedWithDot, digit);
    gtk_entry_set_text(GTK_ENTRY(entryDisplay), numberDisplayedWithDot);
  }
  (strlen(binaryOperator) == 0) ?
    strcpy(operand1, numberDisplayed) :
    strcpy(operand2, numberDisplayed);
}

void on_btnEqual_clicked(void) {
  if (strlen(result) != 0) {
    strcpy(operand1, result);
  }
  if (strlen(operand1) == 0) {
    strcpy(operand1, gtk_entry_get_text(GTK_ENTRY(entryDisplay)));
  }
  if (strlen(binaryOperator) != 0 && strlen(operand2) == 0) {
    strcpy(operand2, gtk_entry_get_text(GTK_ENTRY(entryDisplay)));
  }
  double value1 = atof(operand1);
  double value2 = atof(operand2);
  switch(binaryOperator[0]) {
  case '+':
    sprintf(result, "%.16g", value1 + value2);
    break;
  case '-':
    sprintf(result, "%.16g", value1 - value2);
    break;
  case '*':
    sprintf(result, "%.16g", value1 * value2);
    break;
  case '/':
    sprintf(result, "%.16g", value1 / value2);
    break;
  default:
    sprintf(result, "%.16g", value1);
    break;
  }
  gtk_entry_set_text(GTK_ENTRY(entryDisplay), result);
  flag = true;
}

void on_binaryOperator_clicked(GtkWidget * btn) {
  if (strlen(operand1) == 0) {
    strcpy(operand1, "0");
  } else if (strlen(binaryOperator) != 0 && strlen(operand2) != 0) {
    if (strlen(result) == 0) {
      on_btnEqual_clicked();
    }
    strcpy(operand1, result);
    strclr(operand2);
  }
  strcpy(binaryOperator, gtk_button_get_label(GTK_BUTTON(btn)));
  strclr(result);
  flag = true;
}

void on_unaryOperator_clicked(void (*doCalculate)(char * operand)) {
  if (strlen(result) != 0) {
    (*doCalculate)(result);
    gtk_entry_set_text(GTK_ENTRY(entryDisplay), result);
  } else if (strlen(operand2) != 0) {
    (*doCalculate)(operand2);
    gtk_entry_set_text(GTK_ENTRY(entryDisplay), operand2);
  } else if (strlen(operand1) != 0) {
    (*doCalculate)(operand1);
    gtk_entry_set_text(GTK_ENTRY(entryDisplay), operand1);
  }
}

void doNagative(char * operand) {
  double value = atof(operand);
  sprintf(operand, "%.16g", -value);
}

void doSquareRoot(char * operand) {
  double value = atof(operand);
  sprintf(operand, "%.16g", sqrt(value));
}

void doSquare(char * operand) {
  double value = atof(operand);
  sprintf(operand, "%.16g", value * value);
}

void doReciprocal(char * operand) {
  double value = atof(operand);
  sprintf(operand, "%.16g", 1 / value);
}

void doClearEntry(char * operand) {
  sprintf(operand, "%d", 0);
}

void doBackspace(char * operand) {
  int length = strlen(operand);
  if (length > 1) {
    operand[length-1] = '\0';
  } else if (length == 1) {
    doClearEntry(operand);
  }
}

void on_btnClearEntry_clicked(void)
{ on_unaryOperator_clicked(doClearEntry); }

void on_btnPositiveNegative_clicked(void)
{ on_unaryOperator_clicked(doNagative); }

void on_btnSquareRoot_clicked(void)
{ on_unaryOperator_clicked(doSquareRoot); }

void on_btnSquare_clicked(void)
{ on_unaryOperator_clicked(doSquare); }

void on_btnBackspace_clicked(void)
{ on_unaryOperator_clicked(doBackspace); }

void on_btnReciprocal_clicked(void)
{ on_unaryOperator_clicked(doReciprocal); }

void doPercent(char * operand, double percent) {
  double value = atof(operand);
  value /= 100;
  value *= percent;
  sprintf(operand, "%.16g", value);
}

void on_btnPercent_clicked(void) {
  if (strlen(result) != 0) {
    doPercent(result, 1.0);
    gtk_entry_set_text(GTK_ENTRY(entryDisplay), result);
  } else if (strlen(operand2) != 0) {
    doPercent(operand1, atof(operand2));
    gtk_entry_set_text(GTK_ENTRY(entryDisplay), operand2);
  } else if (strlen(operand1) != 0) {
    (strlen(binaryOperator) == 0) ?
      doPercent(operand1, 1.0) :
      doPercent(operand1, atof(operand1));
    gtk_entry_set_text(GTK_ENTRY(entryDisplay), operand1);
  }
}

void on_btnMemoryClear_clicked(void) {
  strclr(memory);
  gtk_widget_set_sensitive(btnMemoryClear, false);
  gtk_widget_set_sensitive(btnMemoryRecall, false);
  gtk_window_set_title(GTK_WINDOW(window), "计算器");
}

void on_btnMemoryRecall_clicked(void) {
  if (strlen(result) != 0) {
    on_btnAllClear_clicked();
  }
  if (flag) {
    gtk_entry_set_text(GTK_ENTRY(entryDisplay), "0");
    flag = false;
  } else {
    gtk_entry_set_text(GTK_ENTRY(entryDisplay), memory);
  }
  (strlen(binaryOperator) == 0) ?
    strcpy(operand1, gtk_entry_get_text(GTK_ENTRY(entryDisplay))) :
    strcpy(operand2, gtk_entry_get_text(GTK_ENTRY(entryDisplay)));
}

void on_btnMemoryPlus_clicked(void) {
  double value;
  if (strlen(memory) == 0) {
    strcpy(memory, gtk_entry_get_text(GTK_ENTRY(entryDisplay)));
    value = atof(memory);
  } else {
    value = atof(memory);
    value += atof(gtk_entry_get_text(GTK_ENTRY(entryDisplay)));
  }
  if (value == 0.0) {
    gtk_widget_set_sensitive(btnMemoryClear, false);
    gtk_widget_set_sensitive(btnMemoryRecall, false);
    strclr(memory);
    gtk_window_set_title(GTK_WINDOW(window), "计算器");
    flag = false; 
  } else {
    gtk_widget_set_sensitive(btnMemoryClear, true);
    gtk_widget_set_sensitive(btnMemoryRecall, true);
    sprintf(memory, "%.16g", value);
    gtk_window_set_title(GTK_WINDOW(window), "计算器 [M]");
    flag = true;
  }
}

void on_btnMemoryMinus_clicked(void) {
  double value;
  if (strlen(memory) == 0) {
    strcpy(memory, gtk_entry_get_text(GTK_ENTRY(entryDisplay)));
    value = atof(memory);
  } else {
    value = atof(memory);
    value -= atof(gtk_entry_get_text(GTK_ENTRY(entryDisplay)));
  }
  if (value == 0.0) {
    gtk_widget_set_sensitive(btnMemoryClear, false);
    gtk_widget_set_sensitive(btnMemoryRecall, false);
    strclr(memory);
    gtk_window_set_title(GTK_WINDOW(window), "计算器");
    flag = false; 
  } else {
    gtk_widget_set_sensitive(btnMemoryClear, true);
    gtk_widget_set_sensitive(btnMemoryRecall, true);
    sprintf(memory, "%.16g", value);
    gtk_window_set_title(GTK_WINDOW(window), "计算器 [M]");
    flag = true;
  }
}

void prepare_ui(GtkApplication * app) {
  entryDisplay = gtk_entry_new();
  gtk_entry_set_alignment(GTK_ENTRY(entryDisplay), 1.0);
  gtk_entry_set_overwrite_mode(GTK_ENTRY(entryDisplay), false);
  gtk_entry_set_text(GTK_ENTRY(entryDisplay), "0");

  btnMemoryClear      = gtk_button_new_with_label("MC");
  btnMemoryRecall     = gtk_button_new_with_label("MR");
  btnMemoryPlus       = gtk_button_new_with_label("M+");
  btnMemoryMinus      = gtk_button_new_with_label("M-");
  btnPercent          = gtk_button_new_with_label("%%");
  btnSquareRoot       = gtk_button_new_with_label("SQRT");
  btnSquare           = gtk_button_new_with_label("x²");
  btnReciprocal       = gtk_button_new_with_label("1/x");
  btnClearEntry       = gtk_button_new_with_label("CE");
  btnAllClear         = gtk_button_new_with_label("AC");
  btnBackspace        = gtk_button_new_with_label("←");
  btnPlus             = gtk_button_new_with_label("+");
  btnMinus            = gtk_button_new_with_label("-");
  btnTimes            = gtk_button_new_with_label("*");
  btnDivide           = gtk_button_new_with_label("/");
  btnEqual            = gtk_button_new_with_label("=");
  btnPositiveNegative = gtk_button_new_with_label("±");
  btnDot              = gtk_button_new_with_label(".");
  for (int i = 0; i != 10; i++) {
    char buf[2] = {'\0'};
    sprintf(buf, "%d", i);
    btnDigit[i] = gtk_button_new_with_label(buf);
  }
  gtk_widget_set_sensitive(btnMemoryClear, false);
  gtk_widget_set_sensitive(btnMemoryRecall, false);

  gridBasic = gtk_grid_new();
  gtk_grid_set_row_homogeneous(GTK_GRID(gridBasic),  true);
  gtk_grid_set_column_homogeneous(GTK_GRID(gridBasic),  true);
  gtk_grid_set_row_spacing(GTK_GRID(gridBasic), 2);
  gtk_grid_set_column_spacing(GTK_GRID(gridBasic), 2);
  gtk_grid_attach(GTK_GRID(gridBasic), entryDisplay,        0, 0, 4, 1);
  gtk_grid_attach(GTK_GRID(gridBasic), btnMemoryClear,      0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(gridBasic), btnMemoryRecall,     1, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(gridBasic), btnMemoryPlus,       2, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(gridBasic), btnMemoryMinus,      3, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(gridBasic), btnPercent,          0, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(gridBasic), btnSquareRoot,       1, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(gridBasic), btnSquare,           2, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(gridBasic), btnReciprocal,       3, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(gridBasic), btnClearEntry,       0, 3, 1, 1);
  gtk_grid_attach(GTK_GRID(gridBasic), btnAllClear,         1, 3, 1, 1);
  gtk_grid_attach(GTK_GRID(gridBasic), btnBackspace,        2, 3, 1, 1);
  gtk_grid_attach(GTK_GRID(gridBasic), btnDivide,           3, 3, 1, 1);
  gtk_grid_attach(GTK_GRID(gridBasic), btnDigit[7],         0, 4, 1, 1);
  gtk_grid_attach(GTK_GRID(gridBasic), btnDigit[8],         1, 4, 1, 1);
  gtk_grid_attach(GTK_GRID(gridBasic), btnDigit[9],         2, 4, 1, 1);
  gtk_grid_attach(GTK_GRID(gridBasic), btnTimes,            3, 4, 1, 1);
  gtk_grid_attach(GTK_GRID(gridBasic), btnDigit[4],         0, 5, 1, 1);
  gtk_grid_attach(GTK_GRID(gridBasic), btnDigit[5],         1, 5, 1, 1);
  gtk_grid_attach(GTK_GRID(gridBasic), btnDigit[6],         2, 5, 1, 1);
  gtk_grid_attach(GTK_GRID(gridBasic), btnMinus,            3, 5, 1, 1);
  gtk_grid_attach(GTK_GRID(gridBasic), btnDigit[1],         0, 6, 1, 1);
  gtk_grid_attach(GTK_GRID(gridBasic), btnDigit[2],         1, 6, 1, 1);
  gtk_grid_attach(GTK_GRID(gridBasic), btnDigit[3],         2, 6, 1, 1);
  gtk_grid_attach(GTK_GRID(gridBasic), btnPlus,             3, 6, 1, 1);
  gtk_grid_attach(GTK_GRID(gridBasic), btnPositiveNegative, 0, 7, 1, 1);
  gtk_grid_attach(GTK_GRID(gridBasic), btnDigit[0],         1, 7, 1, 1);
  gtk_grid_attach(GTK_GRID(gridBasic), btnDot,              2, 7, 1, 1);
  gtk_grid_attach(GTK_GRID(gridBasic), btnEqual,            3, 7, 1, 1);

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "计算器");
  gtk_container_add(GTK_CONTAINER(window), gridBasic);
  gtk_widget_grab_focus(btnEqual);
  gtk_widget_show_all(window);
}

void prepare_signals(void) {
  for (int i = 0; i != 10; i++) {
    g_signal_connect(btnDigit[i], "clicked",
                     G_CALLBACK(on_btnDigit_clicked), btnDigit[i]);
  }
  g_signal_connect(btnDot, "clicked",
                   G_CALLBACK(on_btnDigit_clicked), btnDot);
  g_signal_connect(btnPlus, "clicked",
                   G_CALLBACK(on_binaryOperator_clicked), btnPlus);
  g_signal_connect(btnMinus, "clicked",
                   G_CALLBACK(on_binaryOperator_clicked), btnMinus);
  g_signal_connect(btnTimes, "clicked",
                   G_CALLBACK(on_binaryOperator_clicked), btnTimes);
  g_signal_connect(btnDivide, "clicked",
                   G_CALLBACK(on_binaryOperator_clicked), btnDivide);
  g_signal_connect(btnEqual, "clicked",
                   G_CALLBACK(on_btnEqual_clicked), NULL);
  g_signal_connect(btnAllClear, "clicked",
                   G_CALLBACK(on_btnAllClear_clicked), NULL);
  g_signal_connect(btnSquareRoot, "clicked",
                   G_CALLBACK(on_btnSquareRoot_clicked), NULL);
  g_signal_connect(btnSquare, "clicked",
                   G_CALLBACK(on_btnSquare_clicked), NULL);
  g_signal_connect(btnReciprocal, "clicked",
                   G_CALLBACK(on_btnReciprocal_clicked), NULL);
  g_signal_connect(btnClearEntry, "clicked",
                   G_CALLBACK(on_btnClearEntry_clicked), NULL);
  g_signal_connect(btnBackspace, "clicked",
                   G_CALLBACK(on_btnBackspace_clicked), NULL);
  g_signal_connect(btnPositiveNegative, "clicked",
                   G_CALLBACK(on_btnPositiveNegative_clicked), NULL);
  g_signal_connect(btnPercent, "clicked",
                   G_CALLBACK(on_btnPercent_clicked), NULL);
  g_signal_connect(btnMemoryClear, "clicked",
                   G_CALLBACK(on_btnMemoryClear_clicked), NULL);
  g_signal_connect(btnMemoryRecall, "clicked",
                   G_CALLBACK(on_btnMemoryRecall_clicked), NULL);
  g_signal_connect(btnMemoryPlus, "clicked",
                   G_CALLBACK(on_btnMemoryPlus_clicked), NULL);
  g_signal_connect(btnMemoryMinus, "clicked",
                   G_CALLBACK(on_btnMemoryMinus_clicked), NULL);
}


void activate(GtkApplication * app,  gpointer user_data) {
  prepare_ui(app);
  prepare_signals();
}