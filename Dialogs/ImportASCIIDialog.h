/***************************************************************************
    File                 : ImportASCIIDialog.h
    Project              : SciDAVis
    --------------------------------------------------------------------
    Copyright            : (C) 2006,2007 by Ion Vasilief,
                           Tilman Benkert, Knut Franke
    Email (use @ for *)  : ion_vasilief*yahoo.fr, thzs*gmx.net,
                           knut.franke*gmx.de
    Description          : Import ASCII file(s) dialog
                           
 ***************************************************************************/


#ifndef IMPORTFILESDIALOG_H
#define IMPORTFILESDIALOG_H

#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QFileDialog>
#include <QPushButton>

class QGroupBox;
class QPushButton;

//! QFileDialog plus generic extension support.
/**
 * This is a simple hack on top of QFileDialog that allows a custom extension widget to be added to
 * the bottom of the dialog. A button is provided for toggling display of this widget on/off.
 *
 * For the placement of button and extension widget, it is assumed that QFileDialog uses a
 * QGridLayout as its top-level layout. Other layouts will probably lead to a strange outlook,
 * although the functionality should stay intact.
 */
class ExtensibleFileDialog : public QFileDialog
{
    Q_OBJECT

    public:
        //! Constructor.
        /**
         * \param parent parent widget (only affects placement of the dialog)
         * \param extended flag: show/hide the advanced options on start-up
         * \param flags window flags
         */
        ExtensibleFileDialog(QWidget *parent=0, bool extended = true, Qt::WindowFlags flags=0);
        //! Set the extension widget to be displayed when the user presses the toggle button.
        void setExtensionWidget(QWidget *extension);

        //! Tells weather the dialog has a valid extension widget
        bool isExtendable(){return d_extension != NULL;};
        bool isExtended(){return d_extension_toggle->isChecked();};
        void setExtended(bool extended){ d_extension_toggle->setChecked(extended);};

    protected:
        //! Button for toggling display of extension on/off.
        QPushButton *d_extension_toggle;

    private slots:
        //! Resize to make/take space for the extension widget.
        void resize(bool extension_on);

    private:
        //! The extension widget
        QWidget *d_extension;
        //! The layout row (of the assumed QGridLayout) used for extensions
        int d_extension_row;
};

//! Import ASCII file(s) dialog
class ImportASCIIDialog: public ExtensibleFileDialog
{
	Q_OBJECT

public:
	//! Possible destinations for the imported data.
	/**
	 * Important: Keep this in sync with the initialization of #d_import_mode in initAdvancedOptions().
	 */
	enum ImportMode {
		NewTables, //!< create a new table for each file (default)
		NewColumns, //!< add each file as new columns to the current table
		NewRows, //!< add each file as new rows to the current table
		Overwrite //!< replace content of current table with the selected file (like importing a single file in previous versions of SciDAVis)
	};

	//! Constructor
	/**
	 * \param importModeEnabled flag: enable/disable import mode combo box
	 * \param parent parent widget (only affects placement of the dialog)
	 * \param extended flag: show/hide the advanced options on start-up
	 * \param flags window flags
	 */
    ImportASCIIDialog(bool importModeEnabled = true, QWidget * parent = nullptr, bool extended = true, Qt::WindowFlags flags = 0 );

	//! Return the selected import mode
	/**
	 * \sa ImportMode
	 */
	ImportMode importMode() const { return (ImportMode) d_import_mode->currentIndex(); }
	//! Return the selected column separator.
    const QChar columnSeparator() const;
	//! Return the number of lines to be skipped at the start of each file.
	int ignoredLines() const { return d_ignored_lines->value(); }
	//! Whether to rename columns based on the first (non-skipped) line.
	bool renameColumns() const { return d_rename_columns->isChecked(); }
	//! Whether to replace sequences of whitespace charecters with a single space.
	bool simplifySpaces() const { return d_simplify_spaces->isChecked(); }
	//! Whether to remove whitespace from beginning and end of lines.
	bool stripSpaces() const { return d_strip_spaces->isChecked(); }
	//! Whether the user wants the import options to be saved.
	bool rememberOptions() const { return d_remember_options->isChecked(); }

	//! Set the selected columns separator.
	void setColumnSeparator(const QString &sep);
	
	//! Returns the locale chosen for conversion to numeric format
	QLocale decimalSeparators();
	//! Returns whether imported data should be interpreted as numbers
	bool convertToNumeric() const { return d_convert_to_numeric->isChecked(); };
		
private slots:
	//! Display help for advanced options.
	void displayHelp();
	//! For #Overwrite mode, allow only one file to be selected.
	void updateImportMode(int mode);

private:
	void closeEvent(QCloseEvent*);
	//! Initialise #d_advanced_options and everything it contains.
	void initAdvancedOptions();

	//! Container widget for all advanced options.
	QGroupBox *d_advanced_options;
	QCheckBox *d_remember_options, *d_convert_to_numeric;
	QPushButton *d_help_button;
	// the actual options
    QComboBox *d_import_mode, *boxDecimalSeparator;
    QLineEdit *d_column_separator;
	QSpinBox *d_ignored_lines;
	QCheckBox *d_rename_columns, *d_simplify_spaces, *d_strip_spaces;
};


#endif
