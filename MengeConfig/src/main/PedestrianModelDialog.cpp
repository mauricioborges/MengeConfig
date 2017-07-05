#include "PedestrianModelDialog.hpp"

#include <QtWidgets/QComboBox.h>
#include <QtWidgets/QDialogButtonBox.h>
#include <QtWidgets/QLabel.h>
#include <QtWidgets/QLayout.h>
#include <QtWidgets/QMessageBox.h>

#include <iostream>

#include <MengeCore/Runtime/SimulatorDB.h>


/////////////////////////////////////////////////////////////////////////////////////////////
//						Implementation of PedestrianModelDialog
/////////////////////////////////////////////////////////////////////////////////////////////

PedestrianModelDialog::PedestrianModelDialog( const Menge::SimulatorDB* simDB, 
                                              const std::string& default_model,
                                              QWidget* parent ) 
  : QDialog( parent ), simDB_( simDB ) {
  this->setWindowTitle( tr( "Select Pedestrian Model" ) );
  model_label_ = new QLabel( tr( "Model" ) );
  model_combobox_ = new QComboBox();
  const int count = static_cast<int>(simDB->modelCount());
  int default_index = default_model.empty() ? 0 : -1;
  for ( int i = 0; i < count; ++i ) {
    const std::string& model_name = simDB->name( i );
    if ( model_name == default_model ) default_index = i;
    model_combobox_->addItem( QString::fromStdString( model_name ) );
  }
  connect( model_combobox_,
           static_cast<void ( QComboBox::* )( int )>( &QComboBox::currentIndexChanged ),
           this, &PedestrianModelDialog::modelChanged );
  if ( default_index < 0 ) {
    QString msg = QString( tr( "Requested default model is not available: " ) )
      + QString( default_model.c_str() );
    QMessageBox::warning( this, tr( "Select Pedestrian Model" ), msg );
    default_index = 0;
  }
  model_combobox_->setCurrentIndex( default_index );

  button_box_ = new QDialogButtonBox( Qt::Horizontal );
  button_box_->addButton( QDialogButtonBox::Ok );
  button_box_->addButton( QDialogButtonBox::Cancel );
  connect( button_box_, &QDialogButtonBox::accepted, this, &QDialog::accept );
  connect( button_box_, &QDialogButtonBox::rejected, this, &QDialog::reject );

  QHBoxLayout* model_layout = new QHBoxLayout;
  model_layout->addWidget( model_label_ );
  model_layout->addWidget( model_combobox_ );
  model_layout->setStretch( 1, 2 );

  QVBoxLayout* main_layout = new QVBoxLayout;
  main_layout->addLayout( model_layout );
  main_layout->addWidget( button_box_ );
  this->setLayout( main_layout );
}

/////////////////////////////////////////////////////////////////////////////////////////////

std::string PedestrianModelDialog::getModelName() const {
  return model_combobox_->currentText().toStdString();
}

void PedestrianModelDialog::modelChanged( int i ) {
  model_combobox_->setToolTip( QString::fromStdString( simDB_->longDescriptions( i ) ) );
}