/*
 * Copyright (c) 2013, Georgia Tech Research Corporation
 * All rights reserved.
 *
 * Author: Michael X. Grey <mxgrey@gatech.edu>
 * Date: May 15, 2013
 *
 * Humanoid Robotics Lab      Georgia Institute of Technology
 * Director: Mike Stilman     http://www.golems.org
 *
 *
 * This file is provided under the following "BSD-style" License:
 *   Redistribution and use in source and binary forms, with or
 *   without modification, are permitted provided that the following
 *   conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 *   CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 *   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 *   USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *   AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *   POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef HUBO_WALK_H
#define HUBO_WALK_H

#include <stdio.h>

#include <QApplication>
#include <QPainter>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>
#include <ros/ros.h>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QRadioButton>
#include <QSpinBox>
#include <QLabel>
#include <QProcess>
#include <QGroupBox>
#include <QButtonGroup>
#include <QProcess>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QClipboard>
#include <QPalette>
#include <QColor>
#include <QThread>
#include <QCheckBox>
#include <QComboBox>
#include <QVector>

#include <vector>

#include <rviz/panel.h>

#include <hubo.h>
#include <hubo-jointparams.h>
#include <Ladderach.h>
//#include <hubo_motion_ros/AchNetworkWidget.h>
#ifdef HAVE_HUBOMZ
#include <zmp-daemon.h>
#endif
#include <balance-daemon.h>
//#include <hubo_motion_ros/include/hubo_motion_ros/AchNetworkWidget.h>


namespace hubo_walk_space
{

class HuboWalkWidget;


class HuboRefreshManager : public QThread
{
Q_OBJECT
public:
    HuboWalkWidget* parentWidget;
    bool alive;
    int waitTime;

protected:
    virtual void run();

protected slots:
    void getWaitTime(int t);

signals:
    void signalRefresh();

};

#ifdef HAVE_HUBOMZ
class ZmpProfile
{
public:
    QString name;
    zmp_cmd_t vals;
};
#endif // HAVE_HUBOMZ

class BalProfile
{
public:
    QString name;
    balance_gains_t vals;
};

// Here we declare our new subclass of rviz::Panel.  Every panel which
// can be added via the Panels/Add_New_Panel menu is a subclass of
// rviz::Panel.
class HuboWalkWidget: public QTabWidget
{
// This class uses Qt slots and is a subclass of QObject, so it needs
// the Q_OBJECT macro.
Q_OBJECT
public:
  // QWidget subclass constructors usually take a parent widget
  // parameter (which usually defaults to 0).  At the same time,
  // pluginlib::ClassLoader creates instances by calling the default
  // constructor (with no arguments).  Taking the parameter and giving
  // a default of 0 lets the default constructor work and also lets
  // someone using the class for something else to pass in a parent
  // widget as they normally would with Qt.
  HuboWalkWidget( QWidget* parent = 0 );
  ~HuboWalkWidget();

  QString groupStyleSheet;

  // Handler for the nested ach daemon process
  QProcess achChannelZmp;
  QProcess achChannelBal;
  QProcess achdZmp;
  bool zmpConnected;
  QProcess achdBal;
  bool balConnected;
  QProcess achdBalCmd;
  QProcess achdLadderCmd;
  QProcess achdLadderPlanner;   

  QProcess achdCorrectionPlanner;
  QProcess achdCorrectionCmd;
  QProcess achdTrajectoryFollowerParams;
  // Update timer
  HuboRefreshManager* refreshManager;
  int getRefreshTime();

  // Ach Channels for sending and receiving data
  ach_channel_t stateChan;
  ach_channel_t zmpCmdChan;
  ach_channel_t balanceParamChan;
  ach_channel_t balanceCmdChan;
  ach_channel_t ladder_plannerInitChan;
  ach_channel_t ladder_trajChan;
  ach_channel_t correction_plannerInitChan;
  ach_channel_t correction_trajChan;
  ach_channel_t traj_params;

  void initializeAchConnections();
//  void initializeAchStructs();
  void sendCommand();
  void sendBalCommand();

  void setIPAddress(int a, int b, int c, int d);
  int getIPAddress(int index);

  // Structs for storing data to transmit
  // TODO: Make the correct structs
#ifdef HAVE_HUBOMZ
  struct zmp_cmd cmd;
#endif // HAVE_HUBOMZ
  struct balance_gains balParams;
  struct balance_cmd balCmd;
  
  // Handling profiles TODO
  //std::vector<zmp_params> profiles;
#ifdef HAVE_HUBOMZ
  QVector<ZmpProfile> zmpProfiles;
  void fillProfile(zmp_cmd_t &vals);
#endif // HAVE_HUBOMZ
  void updateProfileBox();
  QVector<BalProfile> balProfiles;
  void fillbalProfile(balance_gains_t &vals);
  void updatebalProfileBox();
  
  QWidget* commandTab;

    QPushButton* achdConnect;
    QPushButton* achdDisconnect;
    QLabel* statusLabel;
    QLineEdit* ipAddrAEdit;
    QLineEdit* ipAddrBEdit;
    QLineEdit* ipAddrCEdit;
    QLineEdit* ipAddrDEdit;
    
    
    QButtonGroup* radioSelectGroup;
    QRadioButton* guiSelect;
    QRadioButton* joySelect;
    QPushButton* joyLaunch;
    QLabel* joyStatus;
    
    
    QDoubleSpinBox* strideBox;
    QDoubleSpinBox* walkDistanceBox;
    QDoubleSpinBox* rotateAngleBox;
    QSpinBox* maxStepBox;
    QDoubleSpinBox* radiusBox;
    QCheckBox* continuousBox;
    QPushButton* forwardButton;
    QPushButton* leftButton;
    QPushButton* rightButton;
    QPushButton* backButton;
    QPushButton* stopButton;
    QPushButton* turnLeftButton;
    QPushButton* turnRightButton;

    double heightScale;
    QSlider* heightSlide;
    QDoubleSpinBox* comXOffsetBox;
    QPushButton* staticButton;
    QPushButton* balOffButton;

     /////////////// Trajectory follow tab
    QWidget* TrajectoryFollowerTab;

      QComboBox* fileSelect;
      QPushButton* openFile;
      QLineEdit* fileName;

      QPushButton* runButton;
      //Pramod changes
      QPushButton* stopexeButton;

      QPushButton* pauseButton;
      QLabel* pauseStatus;

      QButtonGroup* compSelectGroup;
      QRadioButton* onButton;
      QRadioButton* offButton;
      QPushButton* changeButton; 

    ///////////////
    QWidget* zmpParamTab;
    
      QComboBox* profileSelect;
      QPushButton* saveProfile;
      QPushButton* deleteProfile;
      QPushButton* saveAsProfile;
      QLineEdit* saveAsEdit;
      


      double penalFactor;
      QDoubleSpinBox* lookAheadBox;
      
      QDoubleSpinBox* startupTimeBox;
      QDoubleSpinBox* shutdownTimeBox;
      QDoubleSpinBox* doubleSupportBox;
      QDoubleSpinBox* singleSupportBox;
      QDoubleSpinBox* pauseTimeBox;
      
      QComboBox* walkTypeSelect;
      QComboBox* ikSenseSelect;
      
      QDoubleSpinBox* liftoffHeightBox;
      QDoubleSpinBox* stepDistanceBox;
      QDoubleSpinBox* sideStepDistanceBox;
      QDoubleSpinBox* lateralDistanceBox;
      
      QDoubleSpinBox* comHeightBox;
      QDoubleSpinBox* comIKAngleWeightBox;
    ///////////////

#ifdef HAVE_HUBOMZ
      ik_error_sensitivity int2ikSense(int index);
      int ikSense2int(ik_error_sensitivity ik_sense);
#endif // HAVE_HUBOMZ

    QWidget* balParamTab;

      QComboBox* balProfileSelect;
      QPushButton* savebalProfile;
      QPushButton* deletebalProfile;
      QPushButton* saveAsbalProfile;
      QLineEdit* balSaveAsEdit;

      QDoubleSpinBox* flattenBoxL;
      QDoubleSpinBox* flattenBoxR;
      QDoubleSpinBox* decayBox;
      QDoubleSpinBox* threshMinBoxL;
      QDoubleSpinBox* threshMinBoxR;
      QDoubleSpinBox* threshMaxBoxL;
      QDoubleSpinBox* threshMaxBoxR;

      QDoubleSpinBox* straightenPBoxL;
      QDoubleSpinBox* straightenPBoxR;

      QDoubleSpinBox* straightenRBoxL;
      QDoubleSpinBox* straightenRBoxR;

      QDoubleSpinBox* springBoxL;
      QDoubleSpinBox* springBoxR;
      QDoubleSpinBox* dampBoxL;
      QDoubleSpinBox* dampBoxR;
      QDoubleSpinBox* responseBoxL;
      QDoubleSpinBox* responseBoxR;

      // Gains for nudging the hips using the F/T sensors.
      // Different gains for single and double support.
      QDoubleSpinBox* singleSupportHipNudgeGainBoxP;
      QDoubleSpinBox* singleSupportHipNudgeGainBoxD;
      QDoubleSpinBox* doubleSupportHipNudgeGainBoxP;
      QDoubleSpinBox* doubleSupportHipNudgeGainBoxD;

      QPushButton* updateBalParams;

    QWidget* ladderTab;
 
      QComboBox* ladder_profileSelect;
      QPushButton*  ladder_saveProfile;
      QPushButton*  ladder_deleteProfile;
      QPushButton*  ladder_saveAsProfile;
      QLineEdit*  ladder_saveAsEdit;

      QDoubleSpinBox* xOffsetBox;
      QDoubleSpinBox* yOffsetBox;
      QDoubleSpinBox* jerkPenalBox;

      // for ladder
      QDoubleSpinBox* rung_widthBox;
      QDoubleSpinBox* rung_lengthBox;
      QDoubleSpinBox* rung_1stheightBox;
      QDoubleSpinBox* rung_heightBox;


      QDoubleSpinBox* rail_heightBox;
      QDoubleSpinBox* rail_radiusBox;
      QDoubleSpinBox* stair_numberBox;
      QDoubleSpinBox* stair_slopeBox;

      QDoubleSpinBox* rail_slopeBox;
      QDoubleSpinBox* rail_startBox;
      QDoubleSpinBox* rail_endBox;

      QPushButton* ladder_sendButton;
      QPushButton* ladder_runButton;


    QWidget* correctionTab;
 
      QComboBox* correction_profileSelect;
      QPushButton*  correction_saveProfile;
      QPushButton*  correction_deleteProfile;
      QPushButton*  correction_saveAsProfile;
      QLineEdit*  correction_saveAsEdit;

      QDoubleSpinBox* leftHand_xBox;
      QDoubleSpinBox* leftHand_yBox;
      QDoubleSpinBox* leftHand_zBox;
      QDoubleSpinBox* leftHand_rollBox;
      QDoubleSpinBox* leftHand_pitchBox;
      QDoubleSpinBox* leftHand_yawBox;

      QDoubleSpinBox* rightHand_xBox;
      QDoubleSpinBox* rightHand_yBox;
      QDoubleSpinBox* rightHand_zBox;
      QDoubleSpinBox* rightHand_rollBox;
      QDoubleSpinBox* rightHand_pitchBox;
      QDoubleSpinBox* rightHand_yawBox;

      QDoubleSpinBox* legs_xBox;
      QDoubleSpinBox* legs_yBox;
      QDoubleSpinBox* legs_zBox;
      QDoubleSpinBox* legs_yawBox;

      QPushButton* correction_sendButton;
      QPushButton* correction_runButton;


protected:
  int ipAddrA;
  int ipAddrB;
  int ipAddrC;
  int ipAddrD;

signals:
  void sendWaitTime(int t);

  // Slots will be "connected" to signals in order to respond to user events
protected Q_SLOTS:

  void sendBalParams();

  void handleStaticButton();
  void handleBalOffButton();

  // Handle button events
  void handleProfileSave();
  void handleProfileDelete();
  void handleProfileSaveAs();
  void handleProfileSelect(int index);

  void handlebalProfileSave();
  void handlebalProfileDelete();
  void handlebalProfileSaveAs();
  void handlebalProfileSelect(int index);

  void handleJoyLaunch();
  
  void handleForward();
  void handleLeft();
  void handleTurnLeft();
  void handleRight();
  void handleTurnRight();
  void handleBackward();
  void handleStop();
  void printNotWalkingMessage();

  // Update all state information
  void refreshState();

  // Deal with achd crashes/failures
  void achdExitError(QProcess::ProcessError err);
  void achdExitFinished(int num);
  void achdConnectSlot();
  void achdDisconnectSlot();
  
  void ipEditHandle(const QString &text);

 //for pause and run
  void handleFileRun();
  void handleFilePause() ;

  //Ladder
  void handleLadderSend();
  void sendToLadderPlanner(LadderPlanner current_params);
  void handleLadderRun();

  void handleCorrectionSend();
  void handleCorrectionRun();
  void sendToCorrectionPlanner(CorrectionParams params);
private:

  ///////////////
  void initializeCommandTab();
  void initializeZmpParamTab();
  void initializeBalParamTab();
  void initializeLadderTab();
  void initializeCorrectionTab();
  void initializeTrajectoryFollowerTab();

};

class HuboWalkPanel: public rviz::Panel
{
Q_OBJECT
public:
    HuboWalkPanel(QWidget *parent = 0);

    // Now we declare overrides of rviz::Panel functions for saving and
    // loading data from the config file.  Here the data is the
    // topic name.
    virtual void load( const rviz::Config& config );
    virtual void save( rviz::Config config ) const;

private:

    HuboWalkWidget* content;

};

} // end namespace rviz_plugin_tutorials


#endif // TELEOP_PANEL_H
