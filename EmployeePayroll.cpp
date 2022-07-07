/*
   File: prog6_faw13_cdp139.cpp

   Author: Francis Williams & Collan Parker
   C.S.1428.001
   Lab Section: L05 & L23
   Program: #6
   Due Date: 12/02/19

   This program prints to an output file a title and column headers for a
   payroll report. It then reads an employees work record from an input file.
   The number of overtime hours worked, gross pay, state tax, federal tax,
   and net pay are calculated for each employee. The authors' personal
   identification information followed by the company payroll report is
   printed to the output file. Monetary values are displayed to two decimal
   places.

   An attempt to avoid repetitive code is made.

   An appropriate message is displayed to the console screen if either the
   input or the output file fails to open.

   An appropriate message is written to the console screen informing the
   user of the output file name to which the results have been written.

   The client file (main) calls the following void functions to process
   the data:

	  - printIdInfo prints the authors' personal information - name(s),
        class/section number, lab section number(s), due date - on the first,
        second, third and fourth lines. Two blank lines are left after the
        due date. The lecture section number is displayed in a three-digit
        field with leading zeros shown. The lab section number is displayed
        in a two-digit field with leading zeros shown. The calling routine
        determines the output stream to which the information is directed.
        (Refer to sample output below.)
	  - printReportHeadings prints to the output file the title & column
        headings for the payroll report. One blank line is left after the
        centered report title. Column headers are displayed on two lines.
        (Refer to sample output below.)
      - dataIn reads the employee ID#, hours worked and pay rate from
        an input file storing the values read into parallel arrays. The
        employee ID# is stored in a 1D array of integers. The hours worked
        and the pay rate are stored in the first and second columns of a
        2D array of reals.
	  - overTime calculates the number of overtime hours worked by the
        employee based on a 40 hour work week.
	  - grossPay calculates the employee's gross pay for the week. If the
        employee worked 40 hours or less, gross pay is the hourly pay rate
        multiplied by the number of hours worked; otherwise, the employee
        worked more than 40 hours, and they are paid the regular hourly
        rate for the first 40 hours plus time and a half for any hours
        over 40.
	  - stateTax calculates state taxes owed by the employee, which is
        calculated at a straight 6% of the employee's weekly gross pay.
        (6% is a sample tax rate. The tax rate will vary with each state.)
	  - federalTax calculates federal taxes owed by the employee. If
        weekly gross pay is $400.00 or less, the tax rate is 20%; otherwise,
        the employee's weekly gross pay is more than $400.00, and the tax
        rate is calculated at 20% for the first $400.00 and 31% for any
        amount over $400.00.
        (These rates will vary based on current federal tax regulations.)
	  - netPay calculates the employee's net pay for the week.
        (gross pay minus calculated taxes both state & federal).
	  - printReportData prints to the output file the information for each
        employee in tabular form. Monetary values are displayed to two
        digits of precision. (Refer to sample output below.)
	  - writeFileLocation prints an appropriate message to the console
        screen indicating to the user the name of the output file to which
        the results have been written. The output file name is provided by
        the calling routine.(Refer to sample output below.)


   The following named constants are declared globally:
      - the number of hours in a normal work week (40.00)
      - the state tax rate (0.06)
      - the federal tax rates (0.20; 0.31)
      - the cut off at which federal taxes increase (400.00)
      - parallel array dimensions
      - names used to reference individual columns in the payroll array

   Locally declared named constants include:
      - a string to hold the authors' names
      - a string to hold the authors' lab section number(s)
      - a string to hold the project's due date
      - an integer to hold the lecture section number
      - an integer representing the maximum string length allowed for
        input and output file names which are stored in character arrays
        of that length

==========================================================================

Layout and content of the output are shown in the samples below.

Input (file - prog6_?inp.txt) // '?' represents three-digit lecture section #
       one record per employee / each record containing three numbers

       ID#(integer)  hours worked (real)  pay rate (real)
       ...

Constants: globally declared:
              integer: ROWS
                       COLS
                       {2D array column indices)
                           HRS_WRKD = 0,
                           PAYRATE = 1,
                           OVRTIME = 2,
                           GROSS = 3,
                           ST_TAX = 4,
                           FED_TAX = 5,
                           NETPAY = 6;

              double: CUT_OFF (hours in a work week)
                      STATE_TX_RATE
                      TAX_CUT_OFF (division @ which net pay is taxed higher)
	                  LOW_TAX_RATE
                      HI_TAX_RATE

Constants: locally declared:
              string:  AUTHORS
                       LAB_SECTION
                       DUE_DATE

              integer: LECTURE_SECTION
                       MAX_LENGTH_FN = ?  // filename's maximum length

Output (console):
    Sample Console Output

    Authors' Names
    C.S.1428.?        // '?' represents three-digit lecture section number
    Lab Section: L?   // '?' represents two-digit lab section numbet
    --/--/--          // dashes represent due date, month/day/year
         <blank line>
         <blank line>
    Program results have been written to prog6_?out.txt.

Output (file: prog6_?out.txt): // '?' represents three-digit lecture section #
    Sample File Output

    Authors' Names
    C.S.1428.?        // '?' represents three-digit lecture section number
    Lab Section: L?   // '?' represents two-digit lab section numbet
    --/--/--          // dashes represent due date, month/day/year
         <blank line>
         <blank line>
                         Monthly Payroll Report
         <blank line>
     ID#     Hours    Hourly    Overtime   Gross    State   Federal    Net
            Worked     Rate       Hours     Pay      Tax      Tax      Pay
    1000     51.00      6.55      11.00   370.07    22.20    74.02   273.86
    ...
    1002     26.00     15.00       0.00   390.00    23.40    78.00   288.60
    ...

    =======================================================================

    <Output will vary based on actual input values.>
*/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>     // 4 Code::Blocks

using namespace std;

const int ROWS = 10,    // number of employees
          COLS = 7;

          // 2D array (payroll) column indices
const int HRS_WRKD = 0,
          PAYRATE = 1,
          OVRTIME = 2,
          GROSS = 3,
          ST_TAX = 4,
          FED_TAX = 5,
          NETPAY = 6;

const double CUT_OFF = 40.00,      // work week
             STATE_TX_RATE = 0.06,
             TAX_CUT_OFF = 400.00, // earnings after which taxed at a higher ra
             LOW_TAX_RATE = 0.20,
             HI_TAX_RATE = 0.31;

void printIdInfo ( ostream &, const string, const int, const string,
                   const string );
void printReportHeadings ( ofstream &);
void dataIn ( ifstream &, int [ ], double [ ][COLS]);
void overTime ( double [ ][COLS] );
void grossPay ( double [ ][COLS] );
void stateTax ( double [ ][COLS] );
void federalTax ( double [ ][COLS] );
void netPay ( double [ ][COLS] );
void printReportData ( ofstream &, int [ ], double [ ][COLS] );
void writeFileLocation ( char [ ] );


int main ( )
{
    const string AUTHORS = "Francis Williams & Collan Parker",
                 LAB_SECTION = "Lab Section: L05 & L23",
                 DUE_DATE = "12/02/19";

    const int LECTURE_SECTION = 1,
              MAX_LENGTH_FN = 20;

    char input_filename[MAX_LENGTH_FN + 1] = "prog6_001inp.txt",
         output_filename[MAX_LENGTH_FN + 1] = "prog6_001out.txt";

    int employee[ROWS];          // employee ID#s

    double payroll[ROWS][COLS];  // payroll data

    ifstream fin;
    fin.open( "prog6_001inp.txt" );

    ofstream fout;
    fout.open( "prog6_001out.txt" );

    if ( !fin )
   {
        cout << endl << endl
             << "***Program Terminated.***" << endl << endl
             << "Input file failed to open." << endl;

        return 1;
   }

   if ( !fout )
   {
        cout << endl << endl
             << " ***Program Terminated.*** " << endl << endl
             << "Output file failed to open." << endl;

        fin.close( );

        return 2;
   }

    printIdInfo ( fout, AUTHORS, LECTURE_SECTION, LAB_SECTION, DUE_DATE );
    printReportHeadings ( fout );
    dataIn ( fin, employee, payroll );
    overTime ( payroll );
    grossPay ( payroll );
    stateTax ( payroll );
    federalTax ( payroll );
    netPay ( payroll );
    printReportData ( fout, employee, payroll );
    printIdInfo ( cout, AUTHORS, LECTURE_SECTION, LAB_SECTION, DUE_DATE );
    writeFileLocation ( output_filename );

    fin.close( );

    fout.close( );

    system("PAUSE>NUL");

    return 0;
}

/*
 Function: printIdInfo

 Author(s): Francis Williams & Collan Parker
 C.S.1428.001
 Lab Section: L05 & L23
 Program 6 Function printIdInfo
 12/02/19

 The void function, printIdInfo, displays the authors' identifying information,
 such as names, class and section number, lab section numbers, the due date is
 also displayed. This information will be presented on the first, second,
 third, and fourth lines.

 Receives: out, AUTHORS, LECTURE_SECTION, LAB_SECTION, DUE_DATE
 Constants: AUTHORS
            LECTURE_SECTION
            LAB_SECTION
            DUE_DATE
 Returns: none
*/

void printIdInfo ( ostream &out, const string AUTHORS,
                   const int LECTURE_SECTION, const string LAB_SECTION,
                   const string DUE_DATE )
{
    out << AUTHORS << endl
        << "C.S.1428." << setw(3) << setfill('0') << LECTURE_SECTION << endl
        << LAB_SECTION << endl
        << DUE_DATE << endl
        << endl << endl;
}

/*
 Function: printReportHeadings

 Author(s): Francis Williams & Collan Parker
 C.S.1428.001
 Lab Section: L05 & L23
 Program 6 Function printReportHeadings
 12/02/19

 The void function, printReportHeadings, will print to the output file the
 title and column headings for the monthly payroll report. The column
 headings include ID#, Hours Worked, Hourly Rate, Overtime Hours, Gross Pay,
 State Tax, and Net Pay.

 Receives: output file variable
 Constants: none
 Returns: none
*/

void printReportHeadings ( ofstream &fout ) // TO BE COOTINUED
{
    fout << setfill(' ')
         << setw(42) << "Monthly Payroll Report" << endl << endl
         << "ID#" << setw(10) << "Hours" << setw(10) << "Hourly" << setw(10)
         << "Overtime" << setw(10) << "Gross" << setw(10) << "State"
         << setw(10) << "Federal" << setw(10) <<  "Net" << endl
         << "   " << setw(10) << "Worked" << setw(9) << "Rate" << setw(10)
         << "Hours" << setw(10) << "Pay" << setw(10) << "Tax" << setw(9) << "Tax"
         << setw(12) << "Pay" << endl;
}

/*
 Function: dataIn

 Author(s): Francis Williams & Collan Parker
 C.S.1428.001
 Lab Section: L05 & L23
 Program 6 Function dataIn
 12/02/19

 The void function, dataIn, will read in an employee's ID number, hours worked,
 and pay rate from an input file. Then the ID number will be stored as an
 integer in a one-dimensional array, the hours worked will be stored as a
 double in the first column of a two-dimension array, and pay rate will be
 stored as a double in the second column of the same two-dimensional array.

 Receives: input file variable, protected 1D array of integers
 (employee ID numbers), protected 2D array of reals (payroll information)
 Constants: globally declared integers:
              ROWS - parallel arrays row dimension
              COLS - 2D array column dimension
            column designations is 2D array:
              HRS_WRKD, PAYRATE
 Returns: none
*/

void dataIn ( ifstream &fin, int employee[ ], double payroll[ ][COLS] )
{
    for ( int id = 0; id < ROWS; ++id) // Check to see if this works (2 lines)
          fin >> employee[id] >> payroll[id][HRS_WRKD]
              >> payroll[id][PAYRATE];
}

/*
 Function: overTime

 Author(s): Francis Williams & Collan Parker
 C.S.1428.001
 Lab Section: L05 & L23
 Program 6 Function overTime
 12/02/19

 The void function, overTime, calculates the number of overtime hours an
 individual employee worked based on a 40 hour work week.

 Receives: protected 2D array of reals (payroll information)
 Constants: globally declared integers:
              ROWS - parallel arrays row dimension
              COLS - 2D array column dimension
            column designations is 2D array:
              HRS_WRKD, CUT_OFF, OVRTIME
 Returns: none
*/

void overTime ( double payroll[ ][COLS] )
{
    for ( int id = 0; id < ROWS; ++id)
    {
        if ( payroll[id][HRS_WRKD] > CUT_OFF )
            payroll[id][OVRTIME] = payroll[id][HRS_WRKD] - CUT_OFF;
        else
            payroll[id][OVRTIME] = 0;
    }
}

/*
 Function: grossPay

 Author(s): Francis Williams & Collan Parker
 C.S.1428.001
 Lab Section: L05 & L23
 Program 6 Function grossPay
 12/02/19

 The void function, grossPay, will calculate an individual employee's gross pay
 for the week. If an employee worked less than 40 hours in a week, his/her
 gross pay will be his/her base hourly rate multiplied by his/her hours worked.
 Any employee that worked more than 40 hours in a given week will have a gross
 pay that includes his/her base hourly rate multiplied by 40, plus the
 difference between the number of hours worked and 40, multiplied by 150%
 his/her base pay.

 Below is pseudocode for a gross pay calculation, assuming overtime hours begin
 after 40 hours, and the increased rate of pay for overtime is 150% the base
 hourly wage.

 if hours_worked is greater than fourty;
     gross_pay = ( 40 * hourly_payrate ) +
                 ( ( hours_worked - 40 ) * ( 1.5 * hourly_payrate ) );
 else (hours_worked is less than fourty)
     gross_pay = hourly_payrate * hours_worked;

 Receives: protected 2D array of reals (payroll information)
 Constants: globally declared integers:
              ROWS - parallel arrays row dimension
              COLS - 2D array column dimension
            globally declred reals:
              CUT_OFF
            column designations is 2D array:
              GROSS, PAYRATE, OVRTIME, HRS_WRKD
 Returns: none
*/

void grossPay ( double payroll[ ][COLS] )
{
    for ( int id = 0; id < ROWS; ++id)
    {
        if ( payroll[id][HRS_WRKD] > CUT_OFF ) //Check this too
            payroll[id][GROSS] = CUT_OFF * payroll[id][PAYRATE] +
            ( payroll[id][PAYRATE] * payroll[id][OVRTIME] * 1.5 );
        else
            payroll[id][GROSS] = payroll[id][HRS_WRKD] * payroll[id][PAYRATE];
    }
}

/*
 Function: stateTax

 Author(s): Francis Williams & Collan Parker
 C.S.1428.001
 Lab Section: L05 & L23
 Program 6 Function stateTax
 12/02/19

 The void function, stateTax, calculates the amount owed by the employee in
 state taxes.

 Receives: protected 2D array of reals (payroll information)
 Constants: globally declared integers:
              ROWS - parallel arrays row dimension
              COLS - 2D array column dimension
            column designations is 2D array:
              ST_TAX, GROSS, STATE_TAX_RATE
 Returns: none
*/

void stateTax ( double payroll[ ][COLS] )
{
    for ( int id = 0; id < ROWS; ++id)
        payroll[id][ST_TAX] = payroll[id][GROSS] * STATE_TX_RATE;
}


/*
 Function: federalTax

 Author(s): Francis Williams & Collan Parker
 C.S.1428.001
 Lab Section: L05 & L23
 Program 6 Function federalTax
 12/02/19

 The void function, federalTax, calculates the amount owed by the employee in
 federal taxes. If an employee earned less than 400 dollars in a week, he/she
 will owe 20% of his/her income in federal taxes. Any employee that earned more
 than 400 dollars in a given week will owe 20% of the first 400 dollars in
 federal taxes, plus 31% of the difference between his/her gross pay and 400.

 Below is a pseudocode for calculating the amount owed in federal taxes for an
 individual employee, assuming a tax cut-off of 400.00 and a tax rate of 20%
 for low wage earners and a tax rate of 31% for high wage earners.

 if gross_pay is greater than 400 dollars;
     federal_tax_amount_owed = ( 20% of 400 ) + ( 31% of ( gross_pay - 400 ) );
 else (gross_pay is less than or equal to 400 dollars)
     federal_tax_amount_owed = ( 20% of gross_pay);

 Receives: protected 2D array of reals (payroll information)
 Constants: globally declared integers:
              ROWS - parallel arrays row dimension
              COLS - 2D array column dimension
            column designations in 2D array:
              GROSS, FED_TAX
            globally declared reals:
              TAX_CUT_OFF - - income level at which taxes are increased
              LOW_TAX_RATE
              HI_TAX_RATE
 Returns: none
*/

void federalTax ( double payroll[ ][COLS] )
{
    for ( int id = 0; id < ROWS; ++id)
        if ( payroll[id][GROSS] > TAX_CUT_OFF )
             payroll[id][FED_TAX] = LOW_TAX_RATE * TAX_CUT_OFF +
            ( payroll[id][GROSS] - TAX_CUT_OFF) * HI_TAX_RATE;
        else
            payroll[id][FED_TAX] = LOW_TAX_RATE * payroll[id][GROSS];
}

/*
 Function: netPay

 Author(s): Francis Williams & Collan Parker
 C.S.1428.001
 Lab Section: L05 & L23
 Program 6 Function netPay
 12/02/19

 The void function, netPay, calculates an individual employee's net pay. Net pay
 can be calculated by subtracting the amount owed for state taxes and the amount
 owed for federal taxes from an employee's gross pay.

 Receives: protected 2D array of reals (payroll information)
 Constants: globally declared integers:
              ROWS - parallel arrays row dimension
              COLS - 2D array column dimension
            column designations in 2D array:
              NETPAY, GROSS, ST_TAX, FED_TAX
 Returns: none
*/

void netPay ( double payroll[ ][COLS] )
{
    for ( int id = 0; id < ROWS; ++id)
        payroll[id][NETPAY] = payroll[id][GROSS] - payroll[id][ST_TAX] -
        payroll[id][FED_TAX];
}

/*
 Function: printReportData

 Author(s): Francis Williams & Collan Parker
 C.S.1428.001
 Lab Section: L05 & L23
 Program 6 Function printReportData
 12/02/19

 The void function, printReportData, will display information for each employee
 in tabular form. The displayed information will include: ID number, hours
 worked, hourly rate, overtime hours, gross pay, the amount owed for state
 taxes, the amount owed for federal taxes, and net pay.

 Receives: output file variable, protected 1D array of integers
 (employee ID numbers), protected 2D array of reals (payroll information)
 Constants: globally declared integers:
              COLS - 2D array column dimension
            column designations in 2D array:
              HRS_WRKD, PAYRATE, OVRTIME, GROSS, ST_TAX, FED_TAX, NETPAY

 Returns: none
*/

void printReportData ( ofstream &fout, int employee[],
                       double payroll[ ][COLS] )
{
    for ( int id = 0; id < ROWS; ++id)
        fout << fixed << setprecision(2) << setfill(' ')
             << employee[id] << setw(9) << payroll[id][HRS_WRKD]
             << setw(10) << payroll[id][PAYRATE] << setw(9)
             << payroll[id][OVRTIME] << setw(11) << payroll[id][GROSS]
             << setw(10) << payroll[id][ST_TAX] << setw(9)
             << payroll[id][FED_TAX] << setw(12) << payroll[id][NETPAY]
             << endl;
}

/*
 Function: writeFileLocation

 Author(s): Francis Williams & Collan Parker
 C.S.1428.001
 Lab Section: L05 & L23
 Program 6 Function writeFileLocation
 12/02/19

 The void function, writeFileLocation, prints a confirmation statement to the
 console output screen that informs the user that the report data was written
 to an output file.

 Receives: protected 1D array of characters (output file location)
 Constants: none
 Returns: none
*/

void writeFileLocation ( char output_filename [ ] )
{
    cout << "Program results have been written to " << output_filename << endl;
}

