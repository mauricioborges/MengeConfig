#include "player_controller.hpp"

#include <QtWidgets/QBoxLayout.h>
#include <QtWidgets/QLabel.h>
#include <QtWidgets/QPushButton.h>
#include <QtWidgets/QSlider.h>

#include <iostream>

#include "simulation_player.hpp"

///////////////////////////////////////////////////////////////////////////
//				IMPLEMENTATION FOR PlayerController
///////////////////////////////////////////////////////////////////////////

PlayerController::PlayerController( QWidget* parent ) : QFrame( parent ) {
  _player = new SimulationPlayer( this );
  connect( _player, &SimulationPlayer::finished, this, &PlayerController::playbackFinished );
  connect( _player, &SimulationPlayer::frameChanged, this, &PlayerController::setFrame );

  // Set up the GUI
  QBoxLayout* hLayout = new QHBoxLayout();
  hLayout->setMargin( 0 );

  _play_button = new QPushButton( tr("Play") );
  _play_button->setCheckable( true );
  hLayout->addWidget( _play_button );
  connect( _play_button, &QPushButton::clicked, this, &PlayerController::togglePlayButton );

  _time_slider = new QSlider();
  _time_slider->setOrientation( Qt::Horizontal );
  //connect( _time_slider, &QSlider::valueChanged, this, &PlayerController::setFrame );
  hLayout->addWidget( _time_slider );

  _time_label = new QLabel( "time" );
  hLayout->addWidget( _time_label );

  hLayout->setStretch( 0, 0 );
  hLayout->setStretch( 1, 1 );
  hLayout->setStretch( 2, 0 );

  this->setLayout( hLayout );
  setEnabled( false );
}

///////////////////////////////////////////////////////////////////////////

void PlayerController::setSimulation( Menge::Agents::SimulatorInterface* sim ) {
  setEnabled( sim != nullptr );
  _player->setSimulation( sim );
}

///////////////////////////////////////////////////////////////////////////

void PlayerController::togglePlayButton( bool checked ) {
  if ( checked ) {
    // Start playback 
    _play_button->setText( tr( "Stop" ) );
    _player->start();
  } else {
    // Stop playback
    _play_button->setText( tr( "Play" ) );
    _player->stop();
  }
}

///////////////////////////////////////////////////////////////////////////

void PlayerController::setFrame( int frame_number ) {
  scrubFrame( frame_number );
  if ( _time_slider->maximum() < frame_number ) {
    _time_slider->setMaximum( frame_number );
  }
  _time_slider->blockSignals( true );
  _time_slider->setSliderPosition( frame_number );
  _time_slider->blockSignals( false );
}

///////////////////////////////////////////////////////////////////////////

void PlayerController::scrubFrame( int frame_number ) {
  _time_label->setText( QString::number( frame_number ) );
  if ( _time_slider->sliderPosition() != frame_number ) {
    emit needRedraw();
  }
}

///////////////////////////////////////////////////////////////////////////

void PlayerController::playbackFinished() {
  _play_button->setText( tr( "Play" ) );
  _play_button->setChecked( false );
}