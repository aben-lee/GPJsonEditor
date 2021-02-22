/***************************************************************************
    File                 : ImportASCIIDialog.cpp
    Project              : SciDAVis
    --------------------------------------------------------------------
    Copyright            : (C) 2006,2007 by Ion Vasilief,
                           Tilman Benkert, Knut Franke
    Email (use @ for *)  : ion_vasilief*yahoo.fr, thzs*gmx.net,
                           knut.franke*gmx.de
    Description          : Import ASCII file(s) dialog

 ***************************************************************************/


/***************************************************************************
 *                                                                         *
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the Free Software           *
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor,                    *
 *   Boston, MA  02110-1301  USA                                           *
 *                                                                         *
 ***************************************************************************/

#include "ImportASCIIDialog.h"

#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QRegExp>
#include <QMessageBox>
#include <QCloseEvent>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>

ExtensibleFileDialog::ExtensibleFileDialog(QWidget *parent, bool extended, Qt::WindowFlags flags)
    : QFileDialog(parent, flags)
{
    d_extension = 0;

#if QT_VERSION >= 0x050000
    // needed to extend the Qt5 QFileDialog
    setOption(QFileDialog::DontUseNativeDialog, true);
#endif

    d_extension_toggle = new QPushButton(tr("<< &Advanced"));
    d_extension_toggle->setCheckable(true);
    if (extended)
        d_extension_toggle->toggle();
    d_extension_toggle->hide(); // show only for d_extension != 0

    QGridLayout *main_layout = qobject_cast<QGridLayout*>(layout());
    if (main_layout) {
        d_extension_row = main_layout->rowCount();
        main_layout->addWidget(d_extension_toggle, d_extension_row, main_layout->columnCount()-1);
        main_layout->setRowStretch(d_extension_row, 0);
        main_layout->setRowStretch(d_extension_row+1, 0);
    } else {
        // fallback in case QFileDialog uses a different layout in the future (=> main_layout==0)
        // would probably look a mess, but at least all functions would be accessible
        layout()->addWidget(d_extension_toggle);
    }

    connect(d_extension_toggle, SIGNAL(toggled(bool)), this, SLOT(resize(bool)));
    connect(this, SIGNAL(accepted()), this, SLOT(close()));
    connect(this, SIGNAL(rejected()), this, SLOT(close()));
}

void ExtensibleFileDialog::setExtensionWidget(QWidget *extension)
{
    if (d_extension == extension)
        return;
    if (d_extension) {
        d_extension->hide();
        disconnect(d_extension_toggle, SIGNAL(toggled(bool)));
    }
    d_extension = extension;
    if (!d_extension) {
        d_extension_toggle->hide();
        return;
    }
    d_extension_toggle->show();

    QGridLayout *main_layout = qobject_cast<QGridLayout*>(layout());
    if (main_layout)
        main_layout->addWidget(d_extension, d_extension_row, 0, 2, main_layout->columnCount()-1);
    else
        layout()->addWidget(d_extension);

    d_extension->setVisible(d_extension_toggle->isChecked());
    connect(d_extension_toggle, SIGNAL(toggled(bool)), d_extension, SLOT(setVisible(bool)));
}

void ExtensibleFileDialog::resize(bool extension_on)
{
    QSize geo = size();
    geo.setHeight(geo.height() + (extension_on ? 1 : -1) * (d_extension->sizeHint().height()-44));
    setGeometry(QRect(geometry().topLeft(), geo));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief ImportASCIIDialog::ImportASCIIDialog
/// \param import_mode_enabled
/// \param parent
/// \param extended
/// \param flags
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
ImportASCIIDialog::ImportASCIIDialog( bool import_mode_enabled,QWidget * parent, bool extended, Qt::WindowFlags flags )
: ExtensibleFileDialog(parent, extended, flags )
{
  setWindowTitle(tr("Import File(s)"));

  QStringList filters;
  filters << tr("GPJson files") + " (*.gpjson *.GPJSON)";
  filters << tr("Text files") + " (*.TXT *.txt)";
  filters << tr("Data files") + " (*.DAT *.dat)";
  filters << tr("Comma Separated Values") + " (*.CSV *.csv)";
//  filters << tr("All files") + " (*)";
  setNameFilters( filters );

  setFileMode( QFileDialog::ExistingFile );

  initAdvancedOptions();
  d_import_mode->setEnabled(import_mode_enabled);
  setExtensionWidget(d_advanced_options);

  // get rembered option values

  connect(d_import_mode, SIGNAL(currentIndexChanged(int)), this, SLOT(updateImportMode(int)));
}

void ImportASCIIDialog::initAdvancedOptions()
{
	d_advanced_options = new QGroupBox();
	QVBoxLayout *main_layout = new QVBoxLayout(d_advanced_options);
	QGridLayout *advanced_layout = new QGridLayout();
	main_layout->addLayout(advanced_layout);

	advanced_layout->addWidget(new QLabel(tr("Import each file as: ")), 0, 0);
	d_import_mode = new QComboBox();
	// Important: Keep this in sync with the ImportMode enum.
	d_import_mode->addItem(tr("New Table"));
	d_import_mode->addItem(tr("New Columns"));
	d_import_mode->addItem(tr("New Rows"));
	d_import_mode->addItem(tr("Overwrite Current Table"));
	advanced_layout->addWidget(d_import_mode, 0, 1);

	QLabel *label_column_separator = new QLabel(tr("Separator:"));
	advanced_layout->addWidget(label_column_separator, 1, 0);
    d_column_separator = new QLineEdit(this);
//	d_column_separator->addItem(tr("TAB"));
//	d_column_separator->addItem(tr("SPACE"));
//	d_column_separator->addItem(";" + tr("TAB"));
//	d_column_separator->addItem("," + tr("TAB"));
//	d_column_separator->addItem(";" + tr("SPACE"));
//	d_column_separator->addItem("," + tr("SPACE"));
//	d_column_separator->addItem(";");
//	d_column_separator->addItem(",");
//	d_column_separator->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
//	d_column_separator->setEditable( true );
	advanced_layout->addWidget(d_column_separator, 1, 1);
	// context-sensitive help
	QString help_column_separator = tr("The column separator can be customized. \nThe following special codes can be used:\n\\t for a TAB character \n\\s for a SPACE");
	help_column_separator += "\n"+tr("The separator must not contain the following characters: \n0-9eE.+-");
	d_column_separator->setWhatsThis(help_column_separator);
	label_column_separator->setToolTip(help_column_separator);
	d_column_separator->setToolTip(help_column_separator);
	label_column_separator->setWhatsThis(help_column_separator);

	QLabel *label_ignore_lines = new QLabel(tr( "Ignore first" ));
	advanced_layout->addWidget(label_ignore_lines, 2, 0);
	d_ignored_lines = new QSpinBox();
	d_ignored_lines->setRange( 0, 10000 );
	d_ignored_lines->setSuffix(" " + tr("lines"));
	d_ignored_lines->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
	advanced_layout->addWidget(d_ignored_lines, 2, 1);

	d_rename_columns = new QCheckBox(tr("Use first row to &name columns"));
	advanced_layout->addWidget(d_rename_columns, 0, 2, 1, 2);

	d_strip_spaces = new QCheckBox(tr("&Remove white spaces from line ends"));
	advanced_layout->addWidget(d_strip_spaces, 1, 2, 1, 2);
	// context-sensitive help
	QString help_strip_spaces = tr("By checking this option all white spaces will be \nremoved from the beginning and the end of \nthe lines in the ASCII file.","when translating this check the what's this functions and tool tips to place the '\\n's correctly");
	help_strip_spaces +="\n\n"+tr("Warning: checking this option leads to column \noverlaping if the columns in the ASCII file don't \nhave the same number of rows.");
	help_strip_spaces +="\n"+tr("To avoid this problem you should precisely \ndefine the column separator using TAB and \nSPACE characters.","when translating this check the what's this functions and tool tips to place the '\\n's correctly");
	d_strip_spaces->setWhatsThis(help_strip_spaces);
	d_strip_spaces->setToolTip(help_strip_spaces);

	d_simplify_spaces = new QCheckBox(tr("&Simplify white spaces" ));
	advanced_layout->addWidget(d_simplify_spaces, 2, 2, 1, 2);
	// context-sensitive help
	QString help_simplify_spaces = tr("By checking this option all white spaces will be \nremoved from the beginning and the end of the \nlines and each sequence of internal \nwhitespaces (including the TAB character) will \nbe replaced with a single space.","when translating this check the what's this functions and tool tips to place the '\\n's correctly");
	help_simplify_spaces +="\n\n"+tr("Warning: checking this option leads to column \noverlaping if the columns in the ASCII file don't \nhave the same number of rows.","when translating this check the what's this functions and tool tips to place the '\\n's correctly");
	help_simplify_spaces +="\n"+tr("To avoid this problem you should precisely \ndefine the column separator using TAB and \nSPACE characters.","when translating this check the what's this functions and tool tips to place the '\\n's correctly");
	d_simplify_spaces->setWhatsThis(help_simplify_spaces);
	d_simplify_spaces->setToolTip(help_simplify_spaces);

	d_convert_to_numeric = new QCheckBox(tr("&Numeric data"));
	advanced_layout->addWidget(d_convert_to_numeric, 3, 0, 1, 2);
	
	advanced_layout->addWidget(new QLabel(tr("Decimal Separators")), 3, 1);
	boxDecimalSeparator = new QComboBox();
	boxDecimalSeparator->addItem(tr("default") + " (" + QLocale::system().toString(1000.0, 'f', 1) +")");
	boxDecimalSeparator->addItem(QLocale::c().toString(1000.0, 'f', 1));
	boxDecimalSeparator->addItem(QLocale(QLocale::German).toString(1000.0, 'f', 1));
	boxDecimalSeparator->addItem(QLocale(QLocale::French).toString(1000.0, 'f', 1));
	connect(d_convert_to_numeric, SIGNAL(toggled(bool)), boxDecimalSeparator, SLOT(setEnabled(bool)));
	advanced_layout->addWidget(boxDecimalSeparator, 3, 2);
	
	QHBoxLayout *meta_options_layout = new QHBoxLayout();
	d_remember_options = new QCheckBox(tr("Re&member the above options"));
	meta_options_layout->addWidget(d_remember_options);
	d_help_button = new QPushButton(tr("&Help"));
	connect(d_help_button, SIGNAL(clicked()), this, SLOT(displayHelp()));
	meta_options_layout->addStretch();
	meta_options_layout->addWidget(d_help_button);
	main_layout->addLayout(meta_options_layout);
}

void ImportASCIIDialog::setColumnSeparator(const QString& sep)
{
//	if (sep=="\t")
//		d_column_separator->setCurrentIndex(0);
//	else if (sep==" ")
//		d_column_separator->setCurrentIndex(1);
//	else if (sep==";\t")
//		d_column_separator->setCurrentIndex(2);
//	else if (sep==",\t")
//		d_column_separator->setCurrentIndex(3);
//	else if (sep=="; ")
//		d_column_separator->setCurrentIndex(4);
//	else if (sep==", ")
//		d_column_separator->setCurrentIndex(5);
//	else if (sep==";")
//		d_column_separator->setCurrentIndex(6);
//	else if (sep==",")
//		d_column_separator->setCurrentIndex(7);
//	else
//	{
//		QString separator = sep;
//		d_column_separator->setEditText(separator.replace(" ","\\s").replace("\t","\\t"));
//	}
}

const QChar ImportASCIIDialog::columnSeparator() const
{
//    QString sep = d_column_separator->currentText();

//	if (d_simplify_spaces->isChecked())
//		sep.replace(tr("TAB"), " ", Qt::CaseInsensitive);
//	else
//		sep.replace(tr("TAB"), "\t", Qt::CaseInsensitive);

//	sep.replace(tr("SPACE"), " ", Qt::CaseInsensitive);
//	sep.replace("\\s", " ");
//	sep.replace("\\t", "\t");

	/* TODO
	if (sep.contains(QRegExp("[0-9.eE+-]")))
		QMessageBox::warning(this, tr("Import options error"),
				tr("The separator must not contain the following characters: 0-9eE.+-"));
	*/
    if(d_column_separator->text().isEmpty())
    {
        return  QChar::Tabulation;
    }
    else
    {
        return d_column_separator->text().at(0);
    }
}

void ImportASCIIDialog::displayHelp()
{
	QString s = tr("The column separator can be customized. The following special codes can be used:\n\\t for a TAB character \n\\s for a SPACE");
	s += "\n"+tr("The separator must not contain the following characters: 0-9eE.+-") + "\n\n";
	s += tr( "Remove white spaces from line ends" )+ ":\n";
	s += tr("By checking this option all white spaces will be removed from the beginning and the end of the lines in the ASCII file.") + "\n\n";
	s += tr( "Simplify white spaces" )+ ":\n";
	s += tr("By checking this option each sequence of internal whitespaces (including the TAB character) will be replaced with a single space.");
	s += tr("By checking this option all white spaces will be removed from the beginning and the end of the lines and each sequence of internal whitespaces (including the TAB character) will be replaced with a single space.");

	s +="\n\n"+tr("Warning: using these two last options leads to column overlaping if the columns in the ASCII file don't have the same number of rows.");
	s +="\n"+tr("To avoid this problem you should precisely define the column separator using TAB and SPACE characters.");

	QMessageBox::about(this, tr("Help"), s);
}

void ImportASCIIDialog::updateImportMode(int mode)
{
	if (mode == Overwrite)
		setFileMode( QFileDialog::ExistingFile );
	else
		setFileMode( QFileDialog::ExistingFiles );
}

void ImportASCIIDialog::closeEvent(QCloseEvent* e)
{
	e->accept();
}

QLocale ImportASCIIDialog::decimalSeparators()
{
	QLocale locale;
    switch (boxDecimalSeparator->currentIndex()){
        case 0:
            locale = QLocale::system();
        break;
        case 1:
            locale = QLocale::c();
        break;
        case 2:
            locale = QLocale(QLocale::German);
        break;
        case 3:
            locale = QLocale(QLocale::French);
        break;
    }
	return locale;
}
