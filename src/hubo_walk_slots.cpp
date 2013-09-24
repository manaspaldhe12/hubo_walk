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

#include "hubo_walk.h"
#include <balance-daemon.h>
namespace hubo_walk_space
{

void HuboWalkWidget::handleLadderSend()
{
    LadderPlanner current_params;
    current_params.rung_width=rung_widthBox->value();
    current_params.rung_length=rung_lengthBox->value();

    current_params.rung_spacing=rung_heightBox->value();
    current_params.first_rung_spacing=rung_1stheightBox->value();

    current_params.rail_height=rail_heightBox->value();
    current_params.rail_radius=rail_radiusBox->value();

    current_params.inclination=stair_slopeBox->value();
    current_params.number_of_stairs=stair_numberBox->value();

    sendToLadderPlanner(current_params);
}


void HuboWalkWidget::handleLadderRun()
{
   printf("running Ladder \n");
   ach_channel_t ladder_chan;
   zmp_traj_t plannedTraj;

   ach_channel_t balance_cmd_chan;
   balance_cmd_t ladder_mode;
   memset( &ladder_mode, 0, sizeof(ladder_mode) );
   ladder_mode.cmd_request=BAL_LADDER_CLIMBING;
   memset( &plannedTraj, 0, sizeof(plannedTraj) );
   plannedTraj.count=20000;
   for (int i=0; i<2000; i++){
 	for (int joint=0; joint<HUBO_JOINT_COUNT; joint++){
 		plannedTraj.traj[i].angles[joint]=0;
 		if (joint==LEB){
			plannedTraj.traj[i].angles[LEB]=((float)(i))/2000;
		}
	}
   }


   ach_status_t r = ach_open(&balance_cmd_chan, BALANCE_CMD_CHAN, NULL );
   if( r != ACH_OK )
      fprintf( stderr, "Problem with channel %s: %s (%d)\n",
              BALANCE_CMD_CHAN, ach_result_to_string(r), (int)r );
   std::cout << "balance cmd ach_open result: " << ach_result_to_string(r) << "\n";

   r = ach_put(&balance_cmd_chan, &ladder_mode, sizeof(ladder_mode));
   std::cout << "balance cmd ach_put result: " << ach_result_to_string(r) << "\n";
  
   r = ach_open(&ladder_chan, HUBO_CHAN_LADDER_TRAJ_NAME, NULL );
   if( r != ACH_OK )
      fprintf( stderr, "Problem with channel %s: %s (%d)\n",
              HUBO_CHAN_LADDER_TRAJ_NAME, ach_result_to_string(r), (int)r );
   std::cout << "cm ach_open result: " << ach_result_to_string(r) << "\n";

   r = ach_put(&ladder_chan, &plannedTraj, sizeof(plannedTraj));
   std::cout << "cm ach_put result: " << ach_result_to_string(r) << "\n";


}

void HuboWalkWidget::handleCorrectionSend()
{
     CorrectionParams params;
     params.left_hand_x=leftHand_xBox->value();
     params.left_hand_y=leftHand_yBox->value();
     params.left_hand_z=leftHand_zBox->value();
     params.left_hand_roll=leftHand_rollBox->value();
     params.left_hand_pitch=leftHand_pitchBox->value();
     params.left_hand_yaw=leftHand_yawBox->value();

     params.right_hand_x=rightHand_xBox->value();
     params.right_hand_y=rightHand_yBox->value();
     params.right_hand_z=rightHand_zBox->value();
     params.right_hand_roll=rightHand_rollBox->value();
     params.right_hand_pitch=rightHand_pitchBox->value();
     params.right_hand_yaw=rightHand_yawBox->value();

     params.legs_x=legs_xBox->value();
     params.legs_y=legs_yBox->value();
     params.legs_z=legs_zBox->value();
     params.legs_yaw=legs_yawBox->value();

    sendToCorrectionPlanner(params);
}


void HuboWalkWidget::handleCorrectionRun()
{
   printf("running correction\n");
   
   ach_channel_t correction_chan;
   zmp_traj_t plannedTraj;

   ach_channel_t balance_cmd_chan;
   balance_cmd_t ladder_mode;
   memset( &ladder_mode, 0, sizeof(ladder_mode) );
   ladder_mode.cmd_request=BAL_LADDER_CLIMBING;
   memset( &plannedTraj, 0, sizeof(plannedTraj) );
   plannedTraj.count=20000;
   for (int i=0; i<2000; i++){
 	for (int joint=0; joint<HUBO_JOINT_COUNT; joint++){
 		plannedTraj.traj[i].angles[joint]=0;
 		if (joint==LEB){
			plannedTraj.traj[i].angles[LEB]=((float)(i))/2000;
		}
	}
   }

 
   ach_status_t r = ach_open(&balance_cmd_chan, BALANCE_CMD_CHAN, NULL );
   if( r != ACH_OK )
      fprintf( stderr, "Problem with channel %s: %s (%d)\n",
              BALANCE_CMD_CHAN, ach_result_to_string(r), (int)r );
   std::cout << "balance cmd ach_open result: " << ach_result_to_string(r) << "\n";

   r = ach_put(&balance_cmd_chan, &ladder_mode, sizeof(ladder_mode));
   std::cout << "balance cmd ach_put result: " << ach_result_to_string(r) << "\n";
  
   r = ach_open(&correction_chan, HUBO_CHAN_CORRECTION_TRAJ_NAME, NULL );
   if( r != ACH_OK )
      fprintf( stderr, "Problem with channel %s: %s (%d)\n",
              HUBO_CHAN_CORRECTION_TRAJ_NAME, ach_result_to_string(r), (int)r );
   std::cout << "correction traj ach_open result: " << ach_result_to_string(r) << "\n";

   r = ach_put(&correction_chan, &plannedTraj, sizeof(plannedTraj));
   std::cout << "correction traj ach_put result: " << ach_result_to_string(r) << "\n";



}


void HuboWalkWidget::sendToLadderPlanner(LadderPlanner current_params){
    ach_status_t r = ach_put(&ladder_plannerInitChan, &current_params, sizeof(current_params));
    if( r != ACH_OK )
        std::cout << "Ladder Planner Ach Error: " << ach_result_to_string(r) << std::endl;
 
}


void HuboWalkWidget::sendToCorrectionPlanner(CorrectionParams params){
    ach_status_t r = ach_put(&correction_plannerInitChan, &params, sizeof(params));
    if( r != ACH_OK )
        std::cout << "Correction Planner Ach Error: " << ach_result_to_string(r) << std::endl;
 
}


#ifdef HAVE_HUBOMZ
void HuboWalkWidget::handleForward()
{
    cmd.walk_type = walk_line;
    cmd.cmd_state = WALKING_FORWARD;
    sendCommand();
}

void HuboWalkWidget::handleBackward()
{
    cmd.walk_type = walk_line;
    cmd.cmd_state = WALKING_BACKWARD;
    sendCommand();
}

void HuboWalkWidget::handleLeft()
{
    cmd.walk_type = walk_sidestep;
    cmd.cmd_state = SIDESTEPPING_LEFT;
    sendCommand();
}

void HuboWalkWidget::handleRight()
{
    cmd.walk_type = walk_sidestep;
    cmd.cmd_state = SIDESTEPPING_RIGHT;
    sendCommand();
}

void HuboWalkWidget::handleTurnLeft()
{
    cmd.walk_type = turn_in_place;
    cmd.cmd_state = ROTATING_LEFT;
    sendCommand();
}

void HuboWalkWidget::handleTurnRight()
{
    cmd.walk_type = turn_in_place;
    cmd.cmd_state = ROTATING_RIGHT;
    sendCommand();
}

void HuboWalkWidget::handleStop()
{
    cmd.walk_type = walk_line;
    cmd.cmd_state = STOP;
    sendCommand();
}
#else
void HuboWalkWidget::handleForward()
{
    printNotWalkingMessage();
}

void HuboWalkWidget::handleBackward()
{
    printNotWalkingMessage();
}

void HuboWalkWidget::handleLeft()
{
    printNotWalkingMessage();
}

void HuboWalkWidget::handleRight()
{
    printNotWalkingMessage();
}

void HuboWalkWidget::handleTurnLeft()
{
    printNotWalkingMessage();
}

void HuboWalkWidget::handleTurnRight()
{
    printNotWalkingMessage();
}

void HuboWalkWidget::handleStop()
{
    printNotWalkingMessage();
}

void HuboWalkWidget::printNotWalkingMessage()
{
    std::cout << "\n滚开！この野郎! 당신은나쁜놈! Get lost."
              << "There's no zmp-daemon to send commands to"
              << std::endl;
}


#endif // HAVE_HUBOMZ


void HuboWalkWidget::sendCommand()
{
#ifdef HAVE_HUBOMZ
    fillProfile(cmd);
    cmd.walk_dist = walkDistanceBox->value();
    cmd.sidewalk_dist = walkDistanceBox->value();
    cmd.turnInPlace_angle = rotateAngleBox->value();
    cmd.walk_circle_radius = radiusBox->value();
    cmd.walk_continuous = continuousBox->isChecked();
    ach_status_t r = ach_put(&zmpCmdChan, &cmd, sizeof(cmd));
    if( r != ACH_OK )
        std::cout << "ZMP Command Ach Error: " << ach_result_to_string(r) << std::endl;
#endif // HAVE_HUBOMZ

    balCmd.cmd_request = BAL_ZMP_WALKING;
    sendBalCommand();
    sendBalParams();
}

void HuboWalkWidget::handleStaticButton()
{
    sendBalParams();
    balCmd.cmd_request = BAL_LEGS_ONLY;
    sendBalCommand();
}

void HuboWalkWidget::handleBalOffButton()
{
    balCmd.cmd_request = BAL_READY;
    sendBalCommand();
}

void HuboWalkWidget::sendBalCommand()
{

    balCmd.height = heightSlide->value()/heightScale;
    balCmd.com_x_offset = comXOffsetBox->value();
    ach_status_t r = ach_put( &balanceCmdChan, &balCmd, sizeof(balCmd) );
    if( r != ACH_OK )
        std::cout << "Balance Command Ach Error: " << ach_result_to_string(r) << std::endl;

}

void HuboWalkWidget::sendBalParams()
{
    fillbalProfile(balParams);
    ach_status_t r = ach_put(&balanceParamChan, &balParams, sizeof(balParams));
    if( r != ACH_OK )
        std::cout << "Ach Error: " << ach_result_to_string(r) << std::endl;
}

#ifdef HAVE_HUBOMZ
void HuboWalkWidget::fillProfile(zmp_cmd_t &vals)
{
    vals.max_step_count = maxStepBox->value();
    vals.step_length = stepDistanceBox->value() ;
    vals.halfStanceWidth = lateralDistanceBox->value() ;
    vals.step_height = liftoffHeightBox->value() ;
    vals.sidestep_length = sideStepDistanceBox->value() ;
    vals.com_height = comHeightBox->value() ;
    vals.com_ik_angle_weight = comIKAngleWeightBox->value() ;
    vals.zmpoff_y = yOffsetBox->value() ;
    vals.zmpoff_x = xOffsetBox->value() ;
    vals.lookahead_time = lookAheadBox->value() ;
    vals.walk_startup_time = startupTimeBox->value() ;
    vals.walk_shutdown_time = shutdownTimeBox->value() ;
    vals.min_double_support_time = doubleSupportBox->value() ;
    vals.min_single_support_time = singleSupportBox->value() ;
    vals.min_pause_time = pauseTimeBox->value() ;
    vals.zmp_R = jerkPenalBox->value()*penalFactor ;
    vals.ik_sense = int2ikSense(ikSenseSelect->currentIndex()) ;
}
#endif // HAVE_HUBOMZ

#ifdef HAVE_HUBOMZ
void HuboWalkWidget::handleProfileSave()
{
    int index = profileSelect->currentIndex();
    fillProfile(zmpProfiles[index].vals);
    
    saveAsEdit->clear();
    saveAsEdit->setPlaceholderText("Remember to save your RViz session! (Ctrl-S)");
}

void HuboWalkWidget::handleProfileSelect(int index)
{
    maxStepBox->setValue(zmpProfiles[index].vals.max_step_count );
    stepDistanceBox->setValue(zmpProfiles[index].vals.step_length ) ;
    lateralDistanceBox->setValue(zmpProfiles[index].vals.halfStanceWidth ) ;
    liftoffHeightBox->setValue(zmpProfiles[index].vals.step_height ) ;
    sideStepDistanceBox->setValue(zmpProfiles[index].vals.sidestep_length ) ;
    comHeightBox->setValue(zmpProfiles[index].vals.com_height ) ;
    comIKAngleWeightBox->setValue(zmpProfiles[index].vals.com_ik_angle_weight ) ;
    yOffsetBox->setValue(zmpProfiles[index].vals.zmpoff_y ) ;
    xOffsetBox->setValue(zmpProfiles[index].vals.zmpoff_x ) ;
    lookAheadBox->setValue(zmpProfiles[index].vals.lookahead_time ) ;
    startupTimeBox->setValue(zmpProfiles[index].vals.walk_startup_time ) ;
    shutdownTimeBox->setValue(zmpProfiles[index].vals.walk_shutdown_time ) ;
    doubleSupportBox->setValue(zmpProfiles[index].vals.min_double_support_time ) ;
    singleSupportBox->setValue(zmpProfiles[index].vals.min_single_support_time ) ;
    pauseTimeBox->setValue(zmpProfiles[index].vals.min_pause_time ) ;
    jerkPenalBox->setValue(zmpProfiles[index].vals.zmp_R/penalFactor ) ;
    ikSenseSelect->setCurrentIndex(ikSense2int(zmpProfiles[index].vals.ik_sense));
    
    saveAsEdit->clear();
}

void HuboWalkWidget::handleProfileDelete()
{
    zmpProfiles.remove(profileSelect->currentIndex());
    updateProfileBox();
}

void HuboWalkWidget::handleProfileSaveAs()
{
    ZmpProfile tempProf;
    tempProf.name = saveAsEdit->text();
    fillProfile(tempProf.vals);
    zmpProfiles.append(tempProf);
    updateProfileBox();
    profileSelect->setCurrentIndex(profileSelect->findText(tempProf.name));

    saveAsEdit->clear();
    saveAsEdit->setPlaceholderText("Remember to save your RViz session! (Ctrl-S)");
}

void HuboWalkWidget::updateProfileBox()
{
    profileSelect->clear();
    for(int i=0; i < zmpProfiles.size(); i++)
        profileSelect->addItem(zmpProfiles[i].name);
}
#endif // HAVE_HUBOMZ

///////////// BALANCE

void HuboWalkWidget::fillbalProfile(balance_gains_t &vals)
{
    vals.flattening_gain[LEFT] = flattenBoxL->value();
    vals.flattening_gain[RIGHT] = flattenBoxR->value() ;
    vals.decay_gain[LEFT] = decayBox->value() ;
    vals.decay_gain[RIGHT] = decayBox->value() ;
    vals.force_min_threshold[LEFT] = threshMinBoxL->value();
    vals.force_min_threshold[RIGHT] = threshMinBoxR->value();
    vals.force_max_threshold[LEFT] = threshMaxBoxL->value();
    vals.force_max_threshold[RIGHT] = threshMaxBoxR->value();
    vals.straightening_pitch_gain[LEFT] = straightenPBoxL->value() ;
    vals.straightening_pitch_gain[RIGHT] = straightenPBoxR->value() ;
    vals.straightening_roll_gain[LEFT] = straightenRBoxL->value() ;
    vals.straightening_roll_gain[RIGHT] = straightenRBoxR->value() ;
    vals.spring_gain[LEFT] = springBoxL->value() ;
    vals.spring_gain[RIGHT] = springBoxR->value() ;
    vals.damping_gain[LEFT] = dampBoxL->value() ;
    vals.damping_gain[RIGHT] = dampBoxR->value() ;
    vals.fz_response[LEFT] = responseBoxL->value() ;
    vals.fz_response[RIGHT] = responseBoxR->value() ;
    vals.single_support_hip_nudge_kp = singleSupportHipNudgeGainBoxP->value();
    vals.single_support_hip_nudge_kd = singleSupportHipNudgeGainBoxD->value();
    vals.double_support_hip_nudge_kp = doubleSupportHipNudgeGainBoxP->value();
    vals.double_support_hip_nudge_kd = doubleSupportHipNudgeGainBoxD->value();
}

void HuboWalkWidget::handlebalProfileSave()
{
    int index = balProfileSelect->currentIndex();
    fillbalProfile(balProfiles[index].vals);

    balSaveAsEdit->clear();
    balSaveAsEdit->setPlaceholderText("Remember to save your RViz session! (Ctrl-S)");
}

void HuboWalkWidget::handlebalProfileSelect(int index)
{
    flattenBoxL->setValue( balProfiles[index].vals.flattening_gain[LEFT] );
    flattenBoxR->setValue( balProfiles[index].vals.flattening_gain[RIGHT] );
    decayBox->setValue( balProfiles[index].vals.decay_gain[LEFT] );
    threshMinBoxL->setValue( balProfiles[index].vals.force_min_threshold[LEFT] );
    threshMinBoxR->setValue( balProfiles[index].vals.force_min_threshold[RIGHT] );
    threshMaxBoxL->setValue( balProfiles[index].vals.force_max_threshold[LEFT] );
    threshMaxBoxR->setValue( balProfiles[index].vals.force_max_threshold[RIGHT] );
    straightenPBoxL->setValue( balProfiles[index].vals.straightening_pitch_gain[LEFT] );
    straightenPBoxR->setValue( balProfiles[index].vals.straightening_pitch_gain[RIGHT] );
    straightenRBoxL->setValue( balProfiles[index].vals.straightening_roll_gain[LEFT] );
    straightenRBoxR->setValue( balProfiles[index].vals.straightening_roll_gain[RIGHT] );
    springBoxL->setValue( balProfiles[index].vals.spring_gain[LEFT] );
    springBoxR->setValue( balProfiles[index].vals.spring_gain[RIGHT] );
    dampBoxL->setValue( balProfiles[index].vals.damping_gain[LEFT] );
    dampBoxR->setValue( balProfiles[index].vals.damping_gain[RIGHT] );
    responseBoxL->setValue( balProfiles[index].vals.fz_response[LEFT] );
    responseBoxR->setValue( balProfiles[index].vals.fz_response[RIGHT] );
    singleSupportHipNudgeGainBoxP->setValue( balProfiles[index].vals.single_support_hip_nudge_kp );
    singleSupportHipNudgeGainBoxD->setValue( balProfiles[index].vals.single_support_hip_nudge_kd );
    doubleSupportHipNudgeGainBoxP->setValue( balProfiles[index].vals.double_support_hip_nudge_kp );
    doubleSupportHipNudgeGainBoxD->setValue( balProfiles[index].vals.double_support_hip_nudge_kd );
    balSaveAsEdit->clear();
}

void HuboWalkWidget::handlebalProfileDelete()
{
    balProfiles.remove(balProfileSelect->currentIndex());
    updatebalProfileBox();
}

void HuboWalkWidget::handlebalProfileSaveAs()
{
    BalProfile tempProf;
    tempProf.name = balSaveAsEdit->text();
    fillbalProfile(tempProf.vals);
    balProfiles.append(tempProf);
    updatebalProfileBox();
    balProfileSelect->setCurrentIndex(balProfileSelect->findText(tempProf.name));

    balSaveAsEdit->clear();
    balSaveAsEdit->setPlaceholderText("Remember to save your RViz session! (Ctrl-S)");
}

void HuboWalkWidget::updatebalProfileBox()
{
    balProfileSelect->clear();
    for(int i=0; i < balProfiles.size(); i++)
        balProfileSelect->addItem(balProfiles[i].name);
}


///////////// END: BALANCE




void HuboWalkWidget::handleJoyLaunch()
{
    
}

#ifdef HAVE_HUBOMZ
ik_error_sensitivity HuboWalkWidget::int2ikSense(int index)
{
    switch(index)
    {
    case 0:
        return ik_strict; break;
    case 1:
        return ik_swing_permissive; break;
    case 2:
        return ik_sloppy; break;
    }
}

int HuboWalkWidget::ikSense2int(ik_error_sensitivity ik_sense)
{
    switch(ik_sense)
    {
    case ik_strict:
        return 0; break;
    case ik_swing_permissive:
        return 1; break;
    case ik_sloppy:
        return 2; break;
    }
}
#endif // HAVE_HUBOMZ

void HuboWalkWidget::setIPAddress(int a, int b, int c, int d)
{
    ipAddrA = a;
    ipAddrB = b;
    ipAddrC = c;
    ipAddrD = d;

    ipAddrAEdit->setText(QString::number(ipAddrA));
    ipAddrBEdit->setText(QString::number(ipAddrB));
    ipAddrCEdit->setText(QString::number(ipAddrC));
    ipAddrDEdit->setText(QString::number(ipAddrD));
}

int HuboWalkWidget::getIPAddress(int index)
{
    switch(index)
    {
    case 0:
        return ipAddrA; break;
    case 1:
        return ipAddrB; break;
    case 2:
        return ipAddrC; break;
    case 3:
        return ipAddrD; break;
    }
}


void HuboWalkWidget::ipEditHandle(const QString &text)
{
    ipAddrA = ipAddrAEdit->text().toInt();
    ipAddrB = ipAddrBEdit->text().toInt();
    ipAddrC = ipAddrCEdit->text().toInt();
    ipAddrD = ipAddrDEdit->text().toInt();
}

void HuboWalkWidget::initializeAchConnections()
{
    ach_status_t r;
#ifdef HAVE_HUBOMZ
    achChannelZmp.start("ach mk " + QString::fromLocal8Bit(CHAN_ZMP_CMD_NAME)
                        + " -1 -m 10 -n 8000 -o 666", QIODevice::ReadWrite);
    achChannelZmp.waitForFinished();
    r = ach_open(&zmpCmdChan, CHAN_ZMP_CMD_NAME, NULL);
    if( r != ACH_OK )
        std::cout << "Ach Error: " << ach_result_to_string(r) << std::endl;
#endif // HAVE_HUBOMZ

    achChannelBal.start("ach mk " + QString::fromLocal8Bit(BALANCE_PARAM_CHAN)
                        + " -1 -m 10 -n 8000 -o 666", QIODevice::ReadWrite);
    achChannelBal.waitForFinished();
    r = ach_open(&balanceParamChan, BALANCE_PARAM_CHAN, NULL );
    if( r != ACH_OK )
        std::cout << "Ach Error: " << ach_result_to_string(r) << std::endl;

    achChannelBal.start("ach mk " + QString::fromLocal8Bit(BALANCE_CMD_CHAN)
                        + " -1 -m 10 -n 8000 -o 666", QIODevice::ReadWrite);
    achChannelBal.waitForFinished();
    r = ach_open(&balanceCmdChan, BALANCE_CMD_CHAN, NULL );
    if( r != ACH_OK )
        std::cout << "Ach Error: " << ach_result_to_string(r) << std::endl;

    achChannelBal.start("ach mk " + QString::fromLocal8Bit(LADDER_PLANNERINITCHAN)
                        + " -1 -m 10 -n 6000 -o 666", QIODevice::ReadWrite);
    achChannelBal.waitForFinished();
    r = ach_open(&ladder_plannerInitChan, LADDER_PLANNERINITCHAN, NULL );
    if( r != ACH_OK )
        std::cout << "Ach Error: " << ach_result_to_string(r) << std::endl;


    achChannelBal.start("ach mk " + QString::fromLocal8Bit(HUBO_CHAN_LADDER_TRAJ_NAME)
                        + " -1 -m 3 -n 8000000 -o 666", QIODevice::ReadWrite);
    achChannelBal.waitForFinished();
    r = ach_open(&ladder_trajChan, HUBO_CHAN_LADDER_TRAJ_NAME, NULL );
    if( r != ACH_OK )
        std::cout << "Ach Error: " << ach_result_to_string(r) << std::endl;


    achChannelBal.start("ach mk " + QString::fromLocal8Bit(CORRECTION_PLANNERINITCHAN)
                        + " -1 -m 10 -n 6000 -o 666", QIODevice::ReadWrite);
    achChannelBal.waitForFinished();
    r = ach_open(&correction_plannerInitChan, CORRECTION_PLANNERINITCHAN, NULL );
    if( r != ACH_OK )
        std::cout << "Ach Error: " << ach_result_to_string(r) << std::endl;

    achChannelBal.start("ach mk " + QString::fromLocal8Bit(HUBO_CHAN_CORRECTION_TRAJ_NAME)
                        + " -1 -m 3 -n 8000000 -o 666", QIODevice::ReadWrite);
    achChannelBal.waitForFinished();
    r = ach_open(&correction_trajChan, HUBO_CHAN_LADDER_TRAJ_NAME, NULL );
    if( r != ACH_OK )
        std::cout << "Ach Error: " << ach_result_to_string(r) << std::endl;

}

void HuboWalkWidget::achdConnectSlot()
{
#ifdef HAVE_HUBOMZ
    achdZmp.start("achd push " + QString::number(ipAddrA)
                                 + "." + QString::number(ipAddrB)
                                 + "." + QString::number(ipAddrC)
                                 + "." + QString::number(ipAddrD)
                    + " " + QString::fromLocal8Bit(CHAN_ZMP_CMD_NAME));
    connect(&achdZmp, SIGNAL(finished(int)), this, SLOT(achdExitFinished(int)));
    connect(&achdZmp, SIGNAL(error(QProcess::ProcessError)), this, SLOT(achdExitError(QProcess::ProcessError)));
#endif // HAVE_HUBOMZ

    achdBal.start("achd push " + QString::number(ipAddrA)
                                 + "." + QString::number(ipAddrB)
                                 + "." + QString::number(ipAddrC)
                                 + "." + QString::number(ipAddrD)
                    + " " + QString::fromLocal8Bit(BALANCE_PARAM_CHAN));
    connect(&achdBal, SIGNAL(finished(int)), this, SLOT(achdExitFinished(int)));
    connect(&achdBal, SIGNAL(error(QProcess::ProcessError)), this, SLOT(achdExitError(QProcess::ProcessError)));


    achdBalCmd.start("achd push " + QString::number(ipAddrA)
                                 + "." + QString::number(ipAddrB)
                                 + "." + QString::number(ipAddrC)
                                 + "." + QString::number(ipAddrD)
                    + " " + QString::fromLocal8Bit(BALANCE_CMD_CHAN));
    connect(&achdBalCmd, SIGNAL(finished(int)), this, SLOT(achdExitFinished(int)));
    connect(&achdBalCmd, SIGNAL(error(QProcess::ProcessError)), this, SLOT(achdExitError(QProcess::ProcessError)));


    achdLadderPlanner.start("achd push " + QString::number(ipAddrA)
                                 + "." + QString::number(ipAddrB)
                                 + "." + QString::number(ipAddrC)
                                 + "." + QString::number(ipAddrD)
                    + " " + QString::fromLocal8Bit(LADDER_PLANNERINITCHAN));
    connect(&achdLadderPlanner, SIGNAL(finished(int)), this, SLOT(achdExitFinished(int)));
    connect(&achdLadderPlanner, SIGNAL(error(QProcess::ProcessError)), this, SLOT(achdExitError(QProcess::ProcessError)));


    achdLadderCmd.start("achd push " + QString::number(ipAddrA)
                                 + "." + QString::number(ipAddrB)
                                 + "." + QString::number(ipAddrC)
                                 + "." + QString::number(ipAddrD)
                    + " " + QString::fromLocal8Bit(HUBO_CHAN_LADDER_TRAJ_NAME));
    connect(&achdLadderCmd, SIGNAL(finished(int)), this, SLOT(achdExitFinished(int)));
    connect(&achdLadderCmd, SIGNAL(error(QProcess::ProcessError)), this, SLOT(achdExitError(QProcess::ProcessError)));


    achdCorrectionPlanner.start("achd push " + QString::number(ipAddrA)
                                 + "." + QString::number(ipAddrB)
                                 + "." + QString::number(ipAddrC)
                                 + "." + QString::number(ipAddrD)
                    + " " + QString::fromLocal8Bit(LADDER_PLANNERINITCHAN));
    connect(&achdCorrectionPlanner, SIGNAL(finished(int)), this, SLOT(achdExitFinished(int)));
    connect(&achdCorrectionPlanner, SIGNAL(error(QProcess::ProcessError)), this, SLOT(achdExitError(QProcess::ProcessError)));


    achdCorrectionCmd.start("achd push " + QString::number(ipAddrA)
                                 + "." + QString::number(ipAddrB)
                                 + "." + QString::number(ipAddrC)
                                 + "." + QString::number(ipAddrD)
                    + " " + QString::fromLocal8Bit(HUBO_CHAN_LADDER_TRAJ_NAME));
    connect(&achdCorrectionCmd, SIGNAL(finished(int)), this, SLOT(achdExitFinished(int)));
    connect(&achdCorrectionCmd, SIGNAL(error(QProcess::ProcessError)), this, SLOT(achdExitError(QProcess::ProcessError)));





    statusLabel->setText("Connected");
}

void HuboWalkWidget::achdDisconnectSlot()
{
    achdZmp.kill();
    achdBal.kill();
    achdBalCmd.kill();
    achdLadderCmd.kill();
    statusLabel->setText("Disconnected");
}

void HuboWalkWidget::achdExitError(QProcess::ProcessError err)
{
    statusLabel->setText("Disconnected");
}

void HuboWalkWidget::achdExitFinished(int num)
{
    statusLabel->setText("Disconnected");
}

}
