/*!
 @file    PedestrianModelDialog.hpp
 @brief   Definition of the dialog through the user selects the active pedestrian model.
 */

#pragma once

#include <QtWidgets/QDialog.h>

QT_BEGIN_NAMESPACE
class QLabel;
class QComboBox;
class QDialogButtonBox;
QT_END_NAMESPACE

// Forward declarations
namespace Menge {
  class SimulatorDB;
}

/*!
 *  @brief    Dialog for selecting pedestrian model.
 */
class PedestrianModelDialog : public QDialog {
  Q_OBJECT

public:
  /*!
   *  @brief  Constructor
   *
   *  @param  simDB   The simulator database; available pedestrian models are drawn from this
   *                  database.
   *  @param  parent  The (optional) parent widget.
   */
  PedestrianModelDialog( const Menge::SimulatorDB* simDB, QWidget* parent = 0x0 );

private:
  // Slot for when the model changes -- udpates the tool tip on the combo box.
  void modelChanged( int index );

  // Menge data
  const Menge::SimulatorDB* simDB_;

  // QT dialog components
  QLabel* model_label_;
  QComboBox* model_combobox_;
  QDialogButtonBox* button_box_;
};