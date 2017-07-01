#include "PedestrianModelDialog.hpp"

#include <QtWidgets/QComboBox.h>
#include <QtWidgets/QDialogButtonBox.h>
#include <QtWidgets/QLabel.h>
#include <QtWidgets/QLayout.h>

#include <iostream>

#include <MengeCore/Runtime/SimulatorDB.h>


/////////////////////////////////////////////////////////////////////////////////////////////
//						Implementation of PedestrianModelDialog
/////////////////////////////////////////////////////////////////////////////////////////////

PedestrianModelDialog::PedestrianModelDialog( const Menge::SimulatorDB* simDB, QWidget* parent ) 
  : QDialog( parent ), simDB_( simDB ) {
  this->setWindowTitle( tr( "Select Pedestrian Model" ) );
  model_label_ = new QLabel( tr( "Model" ) );
  model_combobox_ = new QComboBox();
  const int count = static_cast<int>(simDB->modelCount());
  for ( int i = 0; i < count; ++i ) {
    model_combobox_->addItem( QString::fromStdString( simDB->name( i ) ) );
  }
  connect( model_combobox_, static_cast<void ( QComboBox::* )( int )>( &QComboBox::currentIndexChanged ),
           this, &PedestrianModelDialog::modelChanged );
  modelChanged( 0 );

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

void PedestrianModelDialog::modelChanged( int i ) {
  model_combobox_->setToolTip( QString::fromStdString( simDB_->longDescriptions( i ) ) );
}