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

namespace hubo_walk_space
{

HuboWalkPanel::HuboWalkPanel(QWidget *parent)
    : rviz::Panel(parent)
{
    content = new HuboWalkWidget;
    QHBoxLayout* panelLayout = new QHBoxLayout;
    panelLayout->addWidget(content);
    setLayout(panelLayout);
}

HuboWalkWidget::~HuboWalkWidget()
{
    // TODO: Turn these back on
//    refreshManager->alive = false;
//    refreshManager->exit();
}

void HuboWalkPanel::load(const rviz::Config &config)
{
    rviz::Panel::load(config);



    rviz::Config ip_config = config.mapGetChild("HuboIP");
    QVariant a, b, c, d;
    if( !ip_config.mapGetValue("ipAddrA", &a) || !ip_config.mapGetValue("ipAddrB", &b)
     || !ip_config.mapGetValue("ipAddrC", &c) || !ip_config.mapGetValue("ipAddrD", &d))
        ROS_INFO("Loading the IP Address Failed");
    else
        content->setIPAddress(a.toInt(), b.toInt(), c.toInt(), d.toInt());

#ifdef HAVE_HUBOMZ
    rviz::Config p_config = config.mapGetChild("ZmpProfiles");
    QVariant pNum;
    
    if( p_config.mapGetValue("ZmpProfileNum", &pNum) )
    {
        QVariant selectedProfile;
        config.mapGetValue("SelectedZmpProfile", &selectedProfile);
        content->zmpProfiles.resize(size_t(pNum.toInt()));
        
        for(int i=0; i < int(content->zmpProfiles.size()); i++)
        {
            QVariant temp;
            p_config.mapGetValue("ZmpProfileName"+QString::number(i),
                                 &temp);
            content->zmpProfiles[i].name = temp.toString();
            p_config.mapGetValue("max_step_count"+QString::number(i),
                                 &temp);
            content->zmpProfiles[i].vals.max_step_count = size_t(temp.toDouble());
            p_config.mapGetValue("step_length"+QString::number(i),
                                 &temp);
            content->zmpProfiles[i].vals.step_length = temp.toDouble();
            p_config.mapGetValue("halfStanceWidth"+QString::number(i),
                                 &temp);
            content->zmpProfiles[i].vals.halfStanceWidth = temp.toDouble();
            p_config.mapGetValue("step_height"+QString::number(i),
                                 &temp);
            content->zmpProfiles[i].vals.step_height = temp.toDouble();
            p_config.mapGetValue("sidestep_length"+QString::number(i),
                                 &temp);
            content->zmpProfiles[i].vals.sidestep_length = temp.toDouble();
            p_config.mapGetValue("com_height"+QString::number(i),
                                 &temp);
            content->zmpProfiles[i].vals.com_height = temp.toDouble();
            p_config.mapGetValue("com_ik_angle_weight"+QString::number(i),
                                 &temp);
            content->zmpProfiles[i].vals.com_ik_angle_weight = temp.toDouble();
            p_config.mapGetValue("zmpoff_y"+QString::number(i),
                                 &temp);
            content->zmpProfiles[i].vals.zmpoff_y = temp.toDouble();
            p_config.mapGetValue("zmpoff_x"+QString::number(i),
                                 &temp);
            content->zmpProfiles[i].vals.zmpoff_x = temp.toDouble();
            p_config.mapGetValue("lookahead_time"+QString::number(i),
                                 &temp);
            content->zmpProfiles[i].vals.lookahead_time = temp.toDouble();
            p_config.mapGetValue("startup_time"+QString::number(i),
                                 &temp);
            content->zmpProfiles[i].vals.walk_startup_time = temp.toDouble();
            p_config.mapGetValue("shutdown_time"+QString::number(i),
                                 &temp);
            content->zmpProfiles[i].vals.walk_shutdown_time = temp.toDouble();
            p_config.mapGetValue("min_double_support_time"+QString::number(i),
                                 &temp);
            content->zmpProfiles[i].vals.min_double_support_time = temp.toDouble();
            p_config.mapGetValue("min_single_support_time"+QString::number(i),
                                 &temp);
            content->zmpProfiles[i].vals.min_single_support_time = temp.toDouble();
            p_config.mapGetValue("min_pause_time"+QString::number(i),
                                 &temp);
            content->zmpProfiles[i].vals.min_pause_time = temp.toDouble();
            p_config.mapGetValue("zmp_jerk_penalty"+QString::number(i),
                                 &temp);
            content->zmpProfiles[i].vals.zmp_R = temp.toDouble();
            p_config.mapGetValue("ik_sense"+QString::number(i),
                                 &temp);
            content->zmpProfiles[i].vals.ik_sense = ik_error_sensitivity(temp.toInt());
            p_config.mapGetValue("zmp_jerk_penalty"+QString::number(i),
                                 &temp);
            content->zmpProfiles[i].vals.zmp_R = temp.toDouble();
        }
        
        content->updateProfileBox();
        content->profileSelect->setCurrentIndex(selectedProfile.toInt());
    }
    else
        ROS_INFO("No zmp profiles found");
#endif // HAVE_HUBOMZ

    rviz::Config pb_config = config.mapGetChild("BalProfiles");
    QVariant pbNum;

    if( pb_config.mapGetValue("BalProfileNum", &pbNum) )
    {
        QVariant selectedProfile;
        config.mapGetValue("SelectedBalProfile", &selectedProfile);
        content->balProfiles.resize(size_t(pbNum.toInt()));

        for(int i=0; i < int(content->balProfiles.size()); i++)
        {
            QVariant temp;
            pb_config.mapGetValue("BalProfileName"+QString::number(i),
                                 &temp);
            content->balProfiles[i].name = temp.toString();
            pb_config.mapGetValue("flatten_l"+QString::number(i),
                                 &temp);
            content->balProfiles[i].vals.flattening_gain[LEFT] = temp.toDouble();
            pb_config.mapGetValue("flatten_r"+QString::number(i),
                                 &temp);
            content->balProfiles[i].vals.flattening_gain[RIGHT] = temp.toDouble();
            pb_config.mapGetValue("decay_gain"+QString::number(i),
                                  &temp);
            content->balProfiles[i].vals.decay_gain[LEFT] = temp.toDouble();
            content->balProfiles[i].vals.decay_gain[RIGHT] = temp.toDouble();
            pb_config.mapGetValue("thresh_min_l"+QString::number(i),
                                 &temp);
            content->balProfiles[i].vals.force_min_threshold[LEFT] = temp.toDouble();
            pb_config.mapGetValue("thresh_min_r"+QString::number(i),
                                 &temp);
            content->balProfiles[i].vals.force_min_threshold[RIGHT] = temp.toDouble();
            pb_config.mapGetValue("thresh_max_l"+QString::number(i),
                                 &temp);
            content->balProfiles[i].vals.force_max_threshold[LEFT] = temp.toDouble();
            pb_config.mapGetValue("thresh_max_r"+QString::number(i),
                                 &temp);
            content->balProfiles[i].vals.force_max_threshold[RIGHT] = temp.toDouble();
            pb_config.mapGetValue("straightenP_l"+QString::number(i),
                                 &temp);
            content->balProfiles[i].vals.straightening_pitch_gain[LEFT] = temp.toDouble();
            pb_config.mapGetValue("straightenP_r"+QString::number(i),
                                 &temp);
            content->balProfiles[i].vals.straightening_pitch_gain[RIGHT] = temp.toDouble();
            pb_config.mapGetValue("straightenR_l"+QString::number(i),
                                 &temp);
            content->balProfiles[i].vals.straightening_roll_gain[LEFT] = temp.toDouble();
            pb_config.mapGetValue("straightenR_r"+QString::number(i),
                                 &temp);
            content->balProfiles[i].vals.straightening_roll_gain[RIGHT] = temp.toDouble();
            pb_config.mapGetValue("spring_l"+QString::number(i),
                                 &temp);
            content->balProfiles[i].vals.spring_gain[LEFT] = temp.toDouble();
            pb_config.mapGetValue("spring_r"+QString::number(i),
                                 &temp);
            content->balProfiles[i].vals.spring_gain[RIGHT] = temp.toDouble();
            pb_config.mapGetValue("damp_l"+QString::number(i),
                                 &temp);
            content->balProfiles[i].vals.damping_gain[LEFT] = temp.toDouble();
            pb_config.mapGetValue("damp_r"+QString::number(i),
                                 &temp);
            content->balProfiles[i].vals.damping_gain[RIGHT] = temp.toDouble();
            pb_config.mapGetValue("response_l"+QString::number(i),
                                 &temp);
            content->balProfiles[i].vals.fz_response[LEFT] = temp.toDouble();
            pb_config.mapGetValue("response_r"+QString::number(i),
                                 &temp);
            content->balProfiles[i].vals.fz_response[RIGHT] = temp.toDouble();
            pb_config.mapGetValue("single_support_hip_nudge_kp"+QString::number(i),
                                 &temp);
            content->balProfiles[i].vals.single_support_hip_nudge_kp = temp.toDouble();
            pb_config.mapGetValue("single_support_hip_nudge_kd"+QString::number(i),
                                 &temp);
            content->balProfiles[i].vals.single_support_hip_nudge_kd = temp.toDouble();
            pb_config.mapGetValue("double_support_hip_nudge_kp"+QString::number(i),
                                 &temp);
            content->balProfiles[i].vals.double_support_hip_nudge_kp = temp.toDouble();
            pb_config.mapGetValue("double_support_hip_nudge_kd"+QString::number(i),
                                 &temp);
            content->balProfiles[i].vals.double_support_hip_nudge_kd = temp.toDouble();
        }

        content->updatebalProfileBox();
        content->balProfileSelect->setCurrentIndex(selectedProfile.toInt());
    }
    else
        ROS_INFO("No balance profiles found");

    
}

void HuboWalkPanel::save(rviz::Config config) const
{
    rviz::Panel::save(config);
    config.mapSetValue("Class", getClassId());

    rviz::Config ip_config = config.mapMakeChild("HuboIP");

    QVariant a = QVariant(content->getIPAddress(0));
    QVariant b = QVariant(content->getIPAddress(1));
    QVariant c = QVariant(content->getIPAddress(2));
    QVariant d = QVariant(content->getIPAddress(3));

    ip_config.mapSetValue("ipAddrA", a);
    ip_config.mapSetValue("ipAddrB", b);
    ip_config.mapSetValue("ipAddrC", c);
    ip_config.mapSetValue("ipAddrD", d);
    
    QVariant selectedProfile = QVariant(content->profileSelect->currentIndex());
    config.mapSetValue("SelectedZmpProfile", selectedProfile);
    
    rviz::Config p_config = config.mapMakeChild("ZmpProfiles");

#ifdef HAVE_HUBOMZ    
    QVariant pNum = QVariant(int(content->zmpProfiles.size()));
    p_config.mapSetValue("ZmpProfileNum", pNum);
    
    for(int i=0; i < int(content->zmpProfiles.size()); i++)
    {
        content->zmpProfiles[i].name.replace(" ","_");
        p_config.mapSetValue("ZmpProfileName"+QString::number(i),
                             QVariant(content->zmpProfiles[i].name));
        p_config.mapSetValue("max_step_count"+QString::number(i),
                             QVariant(int(content->zmpProfiles[i].vals.max_step_count)));
        p_config.mapSetValue("step_length"+QString::number(i),
                             QVariant(content->zmpProfiles[i].vals.step_length));
        p_config.mapSetValue("halfStanceWidth"+QString::number(i),
                             QVariant(content->zmpProfiles[i].vals.halfStanceWidth));
        p_config.mapSetValue("step_height"+QString::number(i),
                             QVariant(content->zmpProfiles[i].vals.step_height));
        p_config.mapSetValue("sidestep_length"+QString::number(i),
                             QVariant(content->zmpProfiles[i].vals.sidestep_length));
        p_config.mapSetValue("com_height"+QString::number(i),
                             QVariant(content->zmpProfiles[i].vals.com_height));
        p_config.mapSetValue("com_ik_angle_weight"+QString::number(i),
                             QVariant(content->zmpProfiles[i].vals.com_ik_angle_weight));
        p_config.mapSetValue("zmpoff_y"+QString::number(i),
                             QVariant(content->zmpProfiles[i].vals.zmpoff_y));
        p_config.mapSetValue("zmpoff_x"+QString::number(i),
                             QVariant(content->zmpProfiles[i].vals.zmpoff_x));
        p_config.mapSetValue("lookahead_time"+QString::number(i),
                             QVariant(content->zmpProfiles[i].vals.lookahead_time));
        p_config.mapSetValue("startup_time"+QString::number(i),
                             QVariant(content->zmpProfiles[i].vals.walk_startup_time));
        p_config.mapSetValue("shutdown_time"+QString::number(i),
                             QVariant(content->zmpProfiles[i].vals.walk_shutdown_time));
        p_config.mapSetValue("min_double_support_time"+QString::number(i),
                             QVariant(content->zmpProfiles[i].vals.min_double_support_time));
        p_config.mapSetValue("min_single_support_time"+QString::number(i),
                             QVariant(content->zmpProfiles[i].vals.min_single_support_time));
        p_config.mapSetValue("min_pause_time"+QString::number(i),
                             QVariant(content->zmpProfiles[i].vals.min_pause_time));
        p_config.mapSetValue("zmp_jerk_penalty"+QString::number(i),
                             QVariant(content->zmpProfiles[i].vals.zmp_R));
        p_config.mapSetValue("ik_sense"+QString::number(i),
                             QVariant(int(content->zmpProfiles[i].vals.ik_sense)));
    }
#endif // HAVE_HUBOMZ

    QVariant selectedbalProfile = QVariant(content->balProfileSelect->currentIndex());
    config.mapSetValue("SelectedBalProfile", selectedbalProfile);

    rviz::Config pb_config = config.mapMakeChild("BalProfiles");

    QVariant pbNum = QVariant(int(content->balProfiles.size()));
    pb_config.mapSetValue("BalProfileNum", pbNum);

    for(int i=0; i < int(content->balProfiles.size()); i++)
    {
        content->balProfiles[i].name.replace(" ","_");
        pb_config.mapSetValue("BalProfileName"+QString::number(i),
                             QVariant(content->balProfiles[i].name));
        pb_config.mapSetValue("flatten_l"+QString::number(i),
                             QVariant(content->balProfiles[i].vals.flattening_gain[LEFT]));
        pb_config.mapSetValue("flatten_r"+QString::number(i),
                             QVariant(content->balProfiles[i].vals.flattening_gain[RIGHT]));
        pb_config.mapSetValue("decay_gain"+QString::number(i),
                             QVariant(content->balProfiles[i].vals.decay_gain[LEFT]));
        pb_config.mapSetValue("thresh_min_l"+QString::number(i),
                             QVariant(content->balProfiles[i].vals.force_min_threshold[LEFT]));
        pb_config.mapSetValue("thresh_min_r"+QString::number(i),
                             QVariant(content->balProfiles[i].vals.force_min_threshold[RIGHT]));
        pb_config.mapSetValue("thresh_max_l"+QString::number(i),
                             QVariant(content->balProfiles[i].vals.force_max_threshold[LEFT]));
        pb_config.mapSetValue("thresh_max_r"+QString::number(i),
                             QVariant(content->balProfiles[i].vals.force_max_threshold[RIGHT]));
        pb_config.mapSetValue("straightenP_l"+QString::number(i),
                             QVariant(content->balProfiles[i].vals.straightening_pitch_gain[LEFT]));
        pb_config.mapSetValue("straightenP_r"+QString::number(i),
                             QVariant(content->balProfiles[i].vals.straightening_pitch_gain[RIGHT]));
        pb_config.mapSetValue("straightenR_l"+QString::number(i),
                             QVariant(content->balProfiles[i].vals.straightening_roll_gain[LEFT]));
        pb_config.mapSetValue("straightenR_r"+QString::number(i),
                             QVariant(content->balProfiles[i].vals.straightening_roll_gain[RIGHT]));
        pb_config.mapSetValue("spring_l"+QString::number(i),
                             QVariant(content->balProfiles[i].vals.spring_gain[LEFT]));
        pb_config.mapSetValue("spring_r"+QString::number(i),
                             QVariant(content->balProfiles[i].vals.spring_gain[RIGHT]));
        pb_config.mapSetValue("damp_l"+QString::number(i),
                             QVariant(content->balProfiles[i].vals.damping_gain[LEFT]));
        pb_config.mapSetValue("damp_r"+QString::number(i),
                             QVariant(content->balProfiles[i].vals.damping_gain[RIGHT]));
        pb_config.mapSetValue("response_l"+QString::number(i),
                             QVariant(content->balProfiles[i].vals.fz_response[LEFT]));
        pb_config.mapSetValue("response_r"+QString::number(i),
                             QVariant(content->balProfiles[i].vals.fz_response[RIGHT]));
        pb_config.mapSetValue("single_support_hip_nudge_kp"+QString::number(i),
                             QVariant(content->balProfiles[i].vals.single_support_hip_nudge_kp));
        pb_config.mapSetValue("single_support_hip_nudge_kd"+QString::number(i),
                             QVariant(content->balProfiles[i].vals.single_support_hip_nudge_kd));
        pb_config.mapSetValue("double_support_hip_nudge_kp"+QString::number(i),
                             QVariant(content->balProfiles[i].vals.double_support_hip_nudge_kp));
        pb_config.mapSetValue("double_support_hip_nudge_kd"+QString::number(i),
                             QVariant(content->balProfiles[i].vals.double_support_hip_nudge_kd));
    }
    
}

HuboWalkWidget::HuboWalkWidget(QWidget *parent)
    : QTabWidget(parent)
{

    groupStyleSheet = "QGroupBox {"
                      "border: 1px solid gray;"
                      "border-radius: 9px;"
                      "margin-top: 0.5em;"
                      "}"
                      "QGroupBox::title {"
                      "subcontrol-origin: margin;"
                      "left: 10px;"
                      "padding: 0 3px 0 3px;"
                      "}";


#ifdef HAVE_HUBOMZ
    memset(&cmd, 0, sizeof(cmd));
#endif // HAVE_HUBOMZ
    memset(&balParams, 0, sizeof(balParams));
    memset(&balCmd, 0, sizeof(balCmd));

    initializeCommandTab();
    std::cerr << "Command Tab loaded" << std::endl;
    addTab(commandTab, "Command");

#ifdef HAVE_HUBOMZ
    initializeZmpParamTab();
    std::cerr << "ZMP Parameters Tab loaded" << std::endl;
    addTab(zmpParamTab, "ZMP Parameters");
#else
    std::cerr << "ZMP Parameters Tab will NOT be loaded because hubomz is not installed" << std::endl;
#endif // HAVE_HUBOMZ

    initializeBalParamTab();
    std::cerr << "Balance Parameters Tab loaded" << std::endl;
   
    initializeLadderTab();
    std::cerr << "Ladder Parameters Tab loaded" << std::endl;
   
    initializeCorrectionTab();
    std::cerr << "Correction Tab loaded" << std::endl;

    initializeTrajectoryFollowerTab();
    std::cerr<<"Trajectory Follower Tab loaded"<<std::endl;
    addTab(TrajectoryFollowerTab, "Trajectory Follower");
 
    addTab(commandTab, "Command");
    addTab(zmpParamTab, "ZMP Parameters");
    addTab(balParamTab, "Balance Parameters");
    addTab(ladderTab, "Ladder Tab");
    addTab(correctionTab, "Correction Tab");
    addTab(TrajectoryFollowerTab, "Trajectory Follower");
    initializeAchConnections();

//    refreshManager = new HuboRefreshManager;
//    refreshManager->parentWidget = this;
//    connect(this, SIGNAL(sendWaitTime(int)), refreshManager, SLOT(getWaitTime(int)));
//    refreshManager->start();
}

void HuboRefreshManager::run()
{
    alive = true;
    waitTime = 1000;
    connect(this, SIGNAL(signalRefresh()), parentWidget, SLOT(refreshState()));
    while(alive)
    {
        emit signalRefresh();
        this->msleep(waitTime);
    }
    emit finished();
}

void HuboRefreshManager::getWaitTime(int t)
{
    waitTime = t;
}


void HuboWalkWidget::initializeCommandTab()
{
    QSizePolicy pbsize(QSizePolicy::Maximum, QSizePolicy::Maximum);

    // Set up the networking box
    QVBoxLayout* achdLayout = new QVBoxLayout;

    achdConnect = new QPushButton;
    achdConnect->setSizePolicy(pbsize);
    achdConnect->setText("Connect");
    achdConnect->setToolTip("Connect to Hubo's on board computer");
    achdLayout->addWidget(achdConnect, 0, Qt::AlignHCenter | Qt::AlignBottom);
    connect(achdConnect, SIGNAL(clicked()), this, SLOT(achdConnectSlot()));
    
    achdDisconnect = new QPushButton;
    achdDisconnect->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    achdDisconnect->setText("Disconnect");
    achdDisconnect->setToolTip("Disconnect from Hubo's on board computer");
    achdLayout->addWidget(achdDisconnect, 0, Qt::AlignHCenter | Qt::AlignTop);
    connect(achdDisconnect, SIGNAL(clicked()), this, SLOT(achdDisconnectSlot()));

    QHBoxLayout* statusLayout = new QHBoxLayout;
    QLabel* staticLabel = new QLabel;
    staticLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    staticLabel->setText("Status: ");
    statusLayout->addWidget(staticLabel);
    statusLabel = new QLabel;
    statusLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    statusLabel->setText("Not Connected");
    statusLayout->addWidget(statusLabel);
    achdLayout->addLayout(statusLayout);

    QHBoxLayout* networkLayout = new QHBoxLayout;
    networkLayout->addLayout(achdLayout);

    QHBoxLayout* ipLayout = new QHBoxLayout;
    ipLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    ipAddrAEdit = new QLineEdit;
    ipAddrAEdit->setMaxLength(3);
    ipAddrAEdit->setMaximumWidth(50);
    ipAddrAEdit->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    ipAddrAEdit->setToolTip("IP Address for Hubo's on board computer");
    ipLayout->addWidget(ipAddrAEdit);
    connect(ipAddrAEdit, SIGNAL(textEdited(QString)), this, SLOT(ipEditHandle(QString)));
    QLabel* dot1 = new QLabel;
    dot1->setText(".");
    ipLayout->addWidget(dot1);
    ipAddrBEdit = new QLineEdit;
    ipAddrBEdit->setMaxLength(3);
    ipAddrBEdit->setMaximumWidth(50);
    ipAddrBEdit->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    ipAddrBEdit->setToolTip("IP Address for Hubo's on board computer");
    ipLayout->addWidget(ipAddrBEdit);
    connect(ipAddrBEdit, SIGNAL(textEdited(QString)), this, SLOT(ipEditHandle(QString)));
    QLabel* dot2 = new QLabel;
    dot2->setText(".");
    ipLayout->addWidget(dot2);
    ipAddrCEdit = new QLineEdit;
    ipAddrCEdit->setMaxLength(3);
    ipAddrCEdit->setMaximumWidth(50);
    ipAddrCEdit->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    ipAddrCEdit->setToolTip("IP Address for Hubo's on board computer");
    ipLayout->addWidget(ipAddrCEdit);
    connect(ipAddrCEdit, SIGNAL(textEdited(QString)), this, SLOT(ipEditHandle(QString)));
    QLabel* dot3 = new QLabel;
    dot3->setText(".");
    ipLayout->addWidget(dot3);
    ipAddrDEdit = new QLineEdit;
    ipAddrDEdit->setMaxLength(3);
    ipAddrDEdit->setMaximumWidth(50);
    ipAddrDEdit->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    ipAddrDEdit->setToolTip("IP Address for Hubo's on board computer");
    ipLayout->addWidget(ipAddrDEdit);
    connect(ipAddrDEdit, SIGNAL(textEdited(QString)), this, SLOT(ipEditHandle(QString)));

    QLabel* ipTitle = new QLabel;
    ipTitle->setText("IP Address");
    ipTitle->setToolTip("IP Address for Hubo's on board computer");

    QVBoxLayout* ipUpperLayout = new QVBoxLayout;
    ipUpperLayout->addWidget(ipTitle, 0, Qt::AlignLeft | Qt::AlignBottom);
    ipUpperLayout->addLayout(ipLayout);

    networkLayout->addLayout(ipUpperLayout);



    QGroupBox* networkBox = new QGroupBox;
    networkBox->setStyleSheet(groupStyleSheet);
    networkBox->setTitle("Ach Networking");
    networkBox->setLayout(networkLayout);

    setIPAddress(192, 168, 1, 0);
    ////////////////////
    
    QHBoxLayout* controlSelectLayout = new QHBoxLayout;
    radioSelectGroup = new QButtonGroup;
    radioSelectGroup->setExclusive(true);
    
    guiSelect = new QRadioButton;
    guiSelect->setText("GUI");
    guiSelect->setToolTip("Use the GUI below to control Hubo's walking");
    radioSelectGroup->addButton(guiSelect);
    controlSelectLayout->addWidget(guiSelect);
    guiSelect->setChecked(true);
    
    joySelect = new QRadioButton;
    joySelect->setText("Joystick");
    joySelect->setCheckable(false);
    joySelect->setDisabled(true);
    joySelect->setToolTip("Use a handheld controller to control Hubo's walking\n"
                          "(The controller must be plugged into this computer)");
    radioSelectGroup->addButton(joySelect);
    controlSelectLayout->addWidget(joySelect);
    
    QVBoxLayout* joyLayout = new QVBoxLayout;
    joyLaunch = new QPushButton;
    joyLaunch->setSizePolicy(pbsize);
    joyLaunch->setText("Launch Joystick");
    joyLaunch->setToolTip("Begin a program which will read for joystick commands");
    joyLaunch->setDisabled(true);
    joyLayout->addWidget(joyLaunch, 0, Qt::AlignBottom);
    connect(joyLaunch, SIGNAL(clicked()), this, SLOT(handleJoyLaunch()));
    
    QHBoxLayout* joyStatusLayout = new QHBoxLayout;
    QLabel* statlab = new QLabel;
    statlab->setText("Status:");
    joyStatusLayout->addWidget(statlab);
    joyStatus = new QLabel;
    joyStatus->setText("Off");
    joyStatusLayout->addWidget(joyStatus);
    joyStatusLayout->setAlignment(Qt::AlignTop);
    joyLayout->addLayout(joyStatusLayout);
    
    controlSelectLayout->addLayout(joyLayout);
    
    QGroupBox* controlSelectBox = new QGroupBox;
    controlSelectBox->setTitle("Control Method");
    controlSelectBox->setStyleSheet(groupStyleSheet);
    controlSelectBox->setLayout(controlSelectLayout);
    
    
    QHBoxLayout* controlLayout = new QHBoxLayout;

    QGroupBox* zmpCtrlGroup = new QGroupBox;
    zmpCtrlGroup->setTitle("ZMP Commands");
    zmpCtrlGroup->setStyleSheet(groupStyleSheet);
    QHBoxLayout* zmpCtrlLayout = new QHBoxLayout;
    QVBoxLayout* paramLayout = new QVBoxLayout;

    QLabel* walkLab = new QLabel;
    walkLab->setText("Walk Distance:");
    walkLab->setToolTip("Distance to walk (m) after a click");
    paramLayout->addWidget(walkLab, 0, Qt::AlignLeft);
    walkDistanceBox = new QDoubleSpinBox;
    walkDistanceBox->setSingleStep(0.5);
    walkDistanceBox->setValue(0.2);
    walkDistanceBox->setToolTip(walkLab->toolTip());
    paramLayout->addWidget(walkDistanceBox, 0, Qt::AlignLeft);
    
    continuousBox = new QCheckBox;
    continuousBox->setChecked(false);
    continuousBox->setText("Continuous");
    continuousBox->setToolTip("Ignore the walk distance, and walk until Stop is selected");
    continuousBox->setChecked(true);
    continuousBox->setDisabled(true);
    paramLayout->addWidget(continuousBox, 0, Qt::AlignLeft);

    QLabel* rotateAngleLab = new QLabel;
    rotateAngleLab->setText("Turn-in-place\nAngle:");
    rotateAngleLab->setToolTip("Angle (rad) after a click");
    paramLayout->addWidget(rotateAngleLab, 0, Qt::AlignLeft | Qt::AlignBottom);
    rotateAngleBox = new QDoubleSpinBox;
    rotateAngleBox->setSingleStep(0.1);
    rotateAngleBox->setValue(0.5);
    rotateAngleBox->setToolTip(rotateAngleLab->toolTip());
    paramLayout->addWidget(rotateAngleBox, 0, Qt::AlignLeft | Qt::AlignTop);

    QLabel* maxStepLab = new QLabel;
    maxStepLab->setText("Max Steps:");
    maxStepLab->setToolTip("Cut-off for number of steps to take");
    paramLayout->addWidget(maxStepLab, 0, Qt::AlignLeft | Qt::AlignBottom);
    maxStepBox = new QSpinBox;
    maxStepBox->setSingleStep(1);
    maxStepBox->setToolTip(maxStepLab->toolTip());
    maxStepBox->setValue(10);
    paramLayout->addWidget(maxStepBox, 0, Qt::AlignLeft | Qt::AlignTop);
    
    QLabel* turnLab = new QLabel;
    turnLab->setText("Turn Radius:");
    turnLab->setToolTip("Radius of curvature (m) for turning");
    paramLayout->addWidget(turnLab, 0, Qt::AlignLeft | Qt::AlignBottom);
    radiusBox = new QDoubleSpinBox;
    radiusBox->setValue(5.0);
    radiusBox->setMinimum(0);
    radiusBox->setSingleStep(0.1);
    radiusBox->setMaximum(10000);
    radiusBox->setToolTip(turnLab->toolTip());
    paramLayout->addWidget(radiusBox, 0, Qt::AlignLeft | Qt::AlignTop);

    zmpCtrlLayout->addLayout(paramLayout);
    
    QGridLayout* wasdLayout = new QGridLayout;
    wasdLayout->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    
    turnLeftButton = new QPushButton;
    turnLeftButton->setText("  Turn  ");
    turnLeftButton->setSizePolicy(pbsize);
    wasdLayout->addWidget(turnLeftButton, 0, 0, 1, 1, Qt::AlignCenter);
    connect(turnLeftButton, SIGNAL(clicked()), this, SLOT(handleTurnLeft()));
    
    forwardButton = new QPushButton;
    forwardButton->setText("Forward ");
    forwardButton->setSizePolicy(pbsize);
    wasdLayout->addWidget(forwardButton, 0, 1, 1, 1, Qt::AlignCenter);
    connect(forwardButton, SIGNAL(clicked()), this, SLOT(handleForward()));
    
    turnRightButton = new QPushButton;
    turnRightButton->setText("  Turn  ");
    turnRightButton->setSizePolicy(pbsize);
    wasdLayout->addWidget(turnRightButton, 0, 2, 1, 1, Qt::AlignCenter);
    connect(turnRightButton, SIGNAL(clicked()), this, SLOT(handleTurnRight()));
    
    leftButton = new QPushButton;
    leftButton->setText("  Left  ");
    leftButton->setSizePolicy(pbsize);
    wasdLayout->addWidget(leftButton, 1, 0, 1, 1, Qt::AlignCenter);
    connect(leftButton, SIGNAL(clicked()), this, SLOT(handleLeft()));
    
    stopButton = new QPushButton;
    stopButton->setText("  Stop  ");
    stopButton->setSizePolicy(pbsize);
    wasdLayout->addWidget(stopButton, 1, 1, 1, 1, Qt::AlignCenter);
    connect(stopButton, SIGNAL(clicked()), this, SLOT(handleStop()));
    
    rightButton = new QPushButton;
    rightButton->setText(" Right  ");
    rightButton->setSizePolicy(pbsize);
    wasdLayout->addWidget(rightButton, 1, 2, 1, 1, Qt::AlignCenter);
    connect(rightButton, SIGNAL(clicked()), this, SLOT(handleRight()));
    
    
    backButton = new QPushButton;
    backButton->setText("Backward");
    backButton->setSizePolicy(pbsize);
    wasdLayout->addWidget(backButton, 2, 1, 1, 1, Qt::AlignCenter);
    connect(backButton, SIGNAL(clicked()), this, SLOT(handleBackward()));
    
    zmpCtrlLayout->addLayout(wasdLayout);

    zmpCtrlGroup->setLayout(zmpCtrlLayout);
    controlLayout->addWidget(zmpCtrlGroup, 0, Qt::AlignLeft);

    QGroupBox* balCtrlGroup = new QGroupBox;
    balCtrlGroup->setTitle("Static Commands");
    balCtrlGroup->setStyleSheet(groupStyleSheet);
    QVBoxLayout* balLayout = new QVBoxLayout;

    QHBoxLayout* staticCmdsLayout = new QHBoxLayout;

    QVBoxLayout* heightLayout = new QVBoxLayout;
    heightScale = 1000;
    QLabel* heightLabel = new QLabel;
    heightLabel->setText("Height");
    heightLayout->addWidget(heightLabel);
    heightSlide = new QSlider(Qt::Vertical);
    // TODO: Put the following values in a header
    heightSlide->setMaximum((int)((0.33008 + 0.32995 + 0.28947 + 0.0795 - 0.02)*heightScale));
    // ^ Taken from hubo-motion-rt/src/balance-daemon.cpp
    heightSlide->setMinimum((int)((0.25+0.28947+0.0795)*heightScale));
    // ^ Taken from hubo-motion-rt/src/balance-daemon.cpp
    heightSlide->setValue(heightSlide->maximum());
    heightLayout->addWidget(heightSlide);
    connect( heightSlide, SIGNAL(valueChanged(int)), this, SLOT(handleStaticButton()) );

    QVBoxLayout* comXOffsetLayout = new QVBoxLayout;
    QLabel* comXOffsetLab = new QLabel;
    comXOffsetLab->setText("COM-X Offset (m)");
    comXOffsetLayout->addWidget(comXOffsetLab, 0, Qt::AlignCenter);
    comXOffsetBox = new QDoubleSpinBox;
    comXOffsetBox->setDecimals(4);
    comXOffsetBox->setSingleStep(0.001);
    comXOffsetBox->setMinimum(-0.05);
    comXOffsetBox->setMaximum(0.05);
    comXOffsetBox->setValue(0.0);
    comXOffsetLayout->addWidget(comXOffsetBox, 0, Qt::AlignTop);

    staticCmdsLayout->addLayout(heightLayout, 0);
    staticCmdsLayout->addLayout(comXOffsetLayout, 1);
    staticCmdsLayout->setAlignment(Qt::AlignTop);

    balLayout->addLayout(staticCmdsLayout, 0);

    staticButton = new QPushButton;
    staticButton->setText("Balance");
    staticButton->setToolTip("Enter a static balance mode which allows Hubo to squat up and down");
    balLayout->addWidget(staticButton);
    connect( staticButton, SIGNAL(clicked()), this, SLOT(handleStaticButton()) );

    balOffButton = new QPushButton;
    balOffButton->setText(" Off  ");
    balOffButton->setToolTip("Turn off all forms of balance control");
    balLayout->addWidget(balOffButton);
    connect( balOffButton, SIGNAL(clicked()), this, SLOT(handleBalOffButton()) );

    balCtrlGroup->setLayout(balLayout);
    controlLayout->addWidget(balCtrlGroup);
    
    QVBoxLayout* masterCTLayout = new QVBoxLayout;
    masterCTLayout->addWidget(networkBox);
    masterCTLayout->addWidget(controlSelectBox);
    masterCTLayout->addLayout(controlLayout);

    commandTab = new QWidget;
    commandTab->setLayout(masterCTLayout);
}

#ifdef HAVE_HUBOMZ
void HuboWalkWidget::initializeZmpParamTab()
{
    QSizePolicy pbsize(QSizePolicy::Maximum, QSizePolicy::Maximum);
    
    QHBoxLayout* profileLayoutTop = new QHBoxLayout;
    QLabel* profileLab = new QLabel;
    profileLab->setText("Profile:");
    profileLayoutTop->addWidget(profileLab, 0, Qt::AlignVCenter | Qt::AlignRight);
    
    profileSelect = new QComboBox;
    profileLayoutTop->addWidget(profileSelect);
    connect(profileSelect, SIGNAL(currentIndexChanged(int)), this, SLOT(handleProfileSelect(int)));
    
    saveProfile = new QPushButton;
    saveProfile->setSizePolicy(pbsize);
    saveProfile->setText("Save");
    saveProfile->setToolTip("Save the values below into the currently selected profile");
    profileLayoutTop->addWidget(saveProfile);
    connect(saveProfile, SIGNAL(clicked()), this, SLOT(handleProfileSave()));
    
    deleteProfile = new QPushButton;
    deleteProfile->setSizePolicy(pbsize);
    deleteProfile->setText("Delete");
    deleteProfile->setToolTip("Remove the current profile from the list\n"
                              "WARNING: This is permanent!");
    profileLayoutTop->addWidget(deleteProfile);
    connect(deleteProfile, SIGNAL(clicked()), this, SLOT(handleProfileDelete()));
    
    QHBoxLayout* profileLayoutBottom = new QHBoxLayout;
    saveAsProfile = new QPushButton;
    saveAsProfile->setSizePolicy(pbsize);
    saveAsProfile->setText("Save As...");
    saveAsProfile->setToolTip("Save the values below as a new profile with the following name:");
    profileLayoutBottom->addWidget(saveAsProfile);
    connect(saveAsProfile, SIGNAL(clicked()), this, SLOT(handleProfileSaveAs()));
    
    saveAsEdit = new QLineEdit;
    saveAsEdit->setToolTip("Enter a name for a new profile");
    profileLayoutBottom->addWidget(saveAsEdit);
    
    QVBoxLayout* profileLayoutMaster = new QVBoxLayout;
    profileLayoutMaster->addLayout(profileLayoutTop);
    profileLayoutMaster->addLayout(profileLayoutBottom);
    
    
    QVBoxLayout* leftColumn = new QVBoxLayout;
    
    QVBoxLayout* zmpSettingsLayout = new QVBoxLayout;
    zmpSettingsLayout->setAlignment(Qt::AlignCenter);
    
    QHBoxLayout* xoffsetLay = new QHBoxLayout;
    QLabel* xoffsetLab = new QLabel;
    xoffsetLab->setText("X-Offset:");
    xoffsetLab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    xoffsetLab->setToolTip("How far in x should the ZMP be offset from the ankle joint?");
    xoffsetLay->addWidget(xoffsetLab);
    
    xOffsetBox = new QDoubleSpinBox;
    xOffsetBox->setSizePolicy(pbsize);
    xOffsetBox->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    xOffsetBox->setToolTip(xoffsetLab->toolTip());
    xOffsetBox->setDecimals(4);
    xOffsetBox->setValue(0.038);
    xOffsetBox->setSingleStep(0.01);
    xOffsetBox->setMinimum(-10);
    xOffsetBox->setMaximum(10);
    xoffsetLay->addWidget(xOffsetBox);
    
    zmpSettingsLayout->addLayout(xoffsetLay);
    
    QHBoxLayout* yoffsetLay = new QHBoxLayout;
    QLabel* yoffsetLab = new QLabel;
    yoffsetLab->setText("Y-Offset:");
    yoffsetLab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    yoffsetLab->setToolTip("How far in y should the ZMP be offset from the ankle joint?");
    yoffsetLay->addWidget(yoffsetLab);
    
    yOffsetBox = new QDoubleSpinBox;
    yOffsetBox->setSizePolicy(pbsize);
    yOffsetBox->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    yOffsetBox->setToolTip(yoffsetLab->toolTip());
    yOffsetBox->setDecimals(4);
    yOffsetBox->setValue(0);
    yOffsetBox->setSingleStep(0.01);
    yOffsetBox->setMinimum(-10);
    yOffsetBox->setMaximum(10);
    yoffsetLay->addWidget(yOffsetBox);
    
    zmpSettingsLayout->addLayout(yoffsetLay);
    
    QHBoxLayout* jerkLayout = new QHBoxLayout;
    QLabel* jerkPenaltyLab = new QLabel;
    jerkPenaltyLab->setText("Jerk Penalty:");
    jerkPenaltyLab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    jerkPenaltyLab->setToolTip("How much should jerk be penalized by the Preview Controller?");
    jerkLayout->addWidget(jerkPenaltyLab);
    
    jerkPenalBox = new QDoubleSpinBox;
    jerkPenalBox->setSizePolicy(pbsize);
    jerkPenalBox->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    jerkPenalBox->setToolTip(jerkPenaltyLab->toolTip());
    jerkPenalBox->setValue(1);
    jerkPenalBox->setSingleStep(0.1);
    jerkPenalBox->setMinimum(0);
    jerkPenalBox->setMaximum(1000);
    jerkLayout->addWidget(jerkPenalBox);

    penalFactor = 1e-8;
    QLabel* penalFactorLab = new QLabel;
    penalFactorLab->setText(" x "+QString::number(penalFactor,'g'));
    jerkLayout->addWidget(penalFactorLab);
    
    zmpSettingsLayout->addLayout(jerkLayout);
    
    QHBoxLayout* lookAheadLayout = new QHBoxLayout;
    QLabel* lookAheadLab = new QLabel;
    lookAheadLab->setText("Look Ahead Time:");
    lookAheadLab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    lookAheadLab->setToolTip("How far ahead (sec) should the Preview Controller consider?");
    lookAheadLayout->addWidget(lookAheadLab);
    
    lookAheadBox = new QDoubleSpinBox;
    lookAheadBox->setSizePolicy(pbsize);
    lookAheadBox->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    lookAheadBox->setToolTip(lookAheadLab->toolTip());
    lookAheadBox->setValue(2.5);
    lookAheadBox->setSingleStep(0.1);
    lookAheadBox->setMinimum(0);
    lookAheadBox->setMaximum(1000);
    lookAheadLayout->addWidget(lookAheadBox);
    
    zmpSettingsLayout->addLayout(lookAheadLayout);
    
    
    QGroupBox* zmpSettingsBox = new QGroupBox;
    zmpSettingsBox->setTitle("ZMP Settings");
    zmpSettingsBox->setStyleSheet(groupStyleSheet);
    zmpSettingsBox->setLayout(zmpSettingsLayout);
    leftColumn->addWidget(zmpSettingsBox);
    
    
    QVBoxLayout* timeSettingsLayout = new QVBoxLayout;
    timeSettingsLayout->setAlignment(Qt::AlignCenter);

    // Startup time
    QHBoxLayout* startupTimeLay = new QHBoxLayout;
    QLabel* startupTimeLab = new QLabel;
    startupTimeLab->setText("Startup Time:");
    startupTimeLab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    startupTimeLab->setToolTip("How much time (s) should be spent starting up?");
    startupTimeLay->addWidget(startupTimeLab);
    
    startupTimeBox = new QDoubleSpinBox;
    startupTimeBox->setSizePolicy(pbsize);
    startupTimeBox->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    startupTimeBox->setToolTip(startupTimeLab->toolTip());
    startupTimeBox->setDecimals(3);
    startupTimeBox->setValue(0.5);
    startupTimeBox->setSingleStep(0.01);
    startupTimeBox->setMinimum(0);
    startupTimeBox->setMaximum(1000);
    startupTimeLay->addWidget(startupTimeBox);
    
    timeSettingsLayout->addLayout(startupTimeLay);

    // Shutdown time
    QHBoxLayout* shutdownTimeLay = new QHBoxLayout;
    QLabel* shutdownTimeLab = new QLabel;
    shutdownTimeLab->setText("Shutdown Time:");
    shutdownTimeLab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    shutdownTimeLab->setToolTip("How much time (s) should be spent stopping?");
    shutdownTimeLay->addWidget(shutdownTimeLab);
    
    shutdownTimeBox = new QDoubleSpinBox;
    shutdownTimeBox->setSizePolicy(pbsize);
    shutdownTimeBox->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    shutdownTimeBox->setToolTip(shutdownTimeLab->toolTip());
    shutdownTimeBox->setDecimals(3);
    shutdownTimeBox->setValue(0.5);
    shutdownTimeBox->setSingleStep(0.01);
    shutdownTimeBox->setMinimum(0);
    shutdownTimeBox->setMaximum(1000);
    shutdownTimeLay->addWidget(shutdownTimeBox);
    
    timeSettingsLayout->addLayout(shutdownTimeLay);

    // Min double support time
    QHBoxLayout* doubleSupTimeLay = new QHBoxLayout;
    QLabel* doubleSupTimeLab = new QLabel;
    doubleSupTimeLab->setText("Double Support Time:");
    doubleSupTimeLab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    doubleSupTimeLab->setToolTip("How much time (s) should be spent in double support?\n"
                                 "i.e. Having two feet on the ground");
    doubleSupTimeLay->addWidget(doubleSupTimeLab);
    
    doubleSupportBox = new QDoubleSpinBox;
    doubleSupportBox->setSizePolicy(pbsize);
    doubleSupportBox->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    doubleSupportBox->setToolTip(doubleSupTimeLab->toolTip());
    doubleSupportBox->setDecimals(3);
    doubleSupportBox->setValue(0.01);
    doubleSupportBox->setSingleStep(0.01);
    doubleSupportBox->setMinimum(0);
    doubleSupportBox->setMaximum(1000);
    doubleSupTimeLay->addWidget(doubleSupportBox);
    
    timeSettingsLayout->addLayout(doubleSupTimeLay);

    // Min single support time
    QHBoxLayout* singleSupTimeLay = new QHBoxLayout;
    QLabel* singleSupTimeLab = new QLabel;
    singleSupTimeLab->setText("Single Support Time:");
    singleSupTimeLab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    singleSupTimeLab->setToolTip("How much time (s) should be spent in single support?\n"
                                 "i.e. Having one foot on the ground");
    singleSupTimeLay->addWidget(singleSupTimeLab);
    
    singleSupportBox = new QDoubleSpinBox;
    singleSupportBox->setSizePolicy(pbsize);
    singleSupportBox->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    singleSupportBox->setToolTip(singleSupTimeLab->toolTip());
    singleSupportBox->setDecimals(3);
    singleSupportBox->setValue(0.50);
    singleSupportBox->setSingleStep(0.01);
    singleSupportBox->setMinimum(0);
    singleSupportBox->setMaximum(1000);
    singleSupTimeLay->addWidget(singleSupportBox);
    
    timeSettingsLayout->addLayout(singleSupTimeLay);

    // Min pause time
    QHBoxLayout* pauseTimeLay = new QHBoxLayout;
    QLabel* pauseTimeLab = new QLabel;
    pauseTimeLab->setText("Pause Time:");
    pauseTimeLab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    pauseTimeLab->setToolTip("How much time (s) should be spent stationary in double support?");
    pauseTimeLay->addWidget(pauseTimeLab);
    
    pauseTimeBox = new QDoubleSpinBox;
    pauseTimeBox->setSizePolicy(pbsize);
    pauseTimeBox->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    pauseTimeBox->setToolTip(pauseTimeLab->toolTip());
    pauseTimeBox->setDecimals(3);
    pauseTimeBox->setValue(0.00);
    pauseTimeBox->setSingleStep(0.01);
    pauseTimeBox->setMinimum(0);
    pauseTimeBox->setMaximum(1000);
    pauseTimeLay->addWidget(pauseTimeBox);
    
    timeSettingsLayout->addLayout(pauseTimeLay);

    QGroupBox* timeSettingsBox = new QGroupBox;
    timeSettingsBox->setTitle("Time Settings");
    timeSettingsBox->setStyleSheet(groupStyleSheet);
    timeSettingsBox->setLayout(timeSettingsLayout);
    leftColumn->addWidget(timeSettingsBox);
    
    
    
    QVBoxLayout* rightColumn = new QVBoxLayout;
    
    QHBoxLayout* ikSenseLayout = new QHBoxLayout;
    QLabel* ikSenseLab = new QLabel;
    ikSenseLab->setText("IK Sensitivity:");
    ikSenseLayout->addWidget(ikSenseLab, 0, Qt::AlignVCenter | Qt::AlignRight);
    ikSenseSelect = new QComboBox;
    ikSenseLayout->addWidget(ikSenseSelect);//, 0, Qt::AlignVCenter | Qt::AlignLeft);
    
    ikSenseSelect->addItem("Strict");
    ikSenseSelect->addItem("Permissive");
    ikSenseSelect->addItem("Sloppy (DANGEROUS)");
    ikSenseSelect->setCurrentIndex(0);
    
    rightColumn->addLayout(ikSenseLayout);
    
    
    QVBoxLayout* swingSettingsLayout = new QVBoxLayout;
    swingSettingsLayout->setAlignment(Qt::AlignCenter);
    
    QHBoxLayout* footLiftLay = new QHBoxLayout;
    QLabel* footliftLab = new QLabel;
    footliftLab->setText("Foot Liftoff Height:");
    footliftLab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    footliftLab->setToolTip("How heigh (m) should the swing foot lift off the ground?");
    footLiftLay->addWidget(footliftLab);

    liftoffHeightBox = new QDoubleSpinBox;
    liftoffHeightBox->setSizePolicy(pbsize);
    liftoffHeightBox->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    liftoffHeightBox->setToolTip(footliftLab->toolTip());
    liftoffHeightBox->setDecimals(4);
    liftoffHeightBox->setValue(0.04);
    liftoffHeightBox->setSingleStep(0.01);
    liftoffHeightBox->setMinimum(0);
    liftoffHeightBox->setMaximum(2);
    footLiftLay->addWidget(liftoffHeightBox);
    
    swingSettingsLayout->addLayout(footLiftLay);
    
    QHBoxLayout* stepDistanceLay = new QHBoxLayout;
    QLabel* stepDistanceLab = new QLabel;
    stepDistanceLab->setText("Step Distance:");
    stepDistanceLab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    stepDistanceLab->setToolTip("How far forward (m) should the swing foot step?");
    stepDistanceLay->addWidget(stepDistanceLab);

    stepDistanceBox = new QDoubleSpinBox;
    stepDistanceBox->setSizePolicy(pbsize);
    stepDistanceBox->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    stepDistanceBox->setToolTip(stepDistanceLab->toolTip());
    stepDistanceBox->setDecimals(4);
    stepDistanceBox->setValue(0.1);
    stepDistanceBox->setSingleStep(0.01);
    stepDistanceBox->setMinimum(0);
    stepDistanceBox->setMaximum(5);
    stepDistanceLay->addWidget(stepDistanceBox);
    
    swingSettingsLayout->addLayout(stepDistanceLay);

    QHBoxLayout* sideStepDistanceLay = new QHBoxLayout;
    QLabel* sideStepDistanceLab = new QLabel;
    sideStepDistanceLab->setText("Side Step Distance:");
    sideStepDistanceLab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    sideStepDistanceLab->setToolTip("How far sideways (m) should the swing foot step?");
    sideStepDistanceLay->addWidget(sideStepDistanceLab);

    sideStepDistanceBox = new QDoubleSpinBox;
    sideStepDistanceBox->setSizePolicy(pbsize);
    sideStepDistanceBox->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    sideStepDistanceBox->setToolTip(sideStepDistanceLab->toolTip());
    sideStepDistanceBox->setDecimals(4);
    sideStepDistanceBox->setValue(0.04);
    sideStepDistanceBox->setSingleStep(0.01);
    sideStepDistanceBox->setMinimum(0);
    sideStepDistanceBox->setMaximum(5);
    sideStepDistanceLay->addWidget(sideStepDistanceBox);

    swingSettingsLayout->addLayout(sideStepDistanceLay);

    
    QHBoxLayout* lateralDistanceLay = new QHBoxLayout;
    QLabel* lateralDistanceLab = new QLabel;
    lateralDistanceLab->setText("Lateral Distance:");
    lateralDistanceLab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    lateralDistanceLab->setToolTip("How far from the center (m) should the swing foot be placed?");
    lateralDistanceLay->addWidget(lateralDistanceLab);

    lateralDistanceBox = new QDoubleSpinBox;
    lateralDistanceBox->setSizePolicy(pbsize);
    lateralDistanceBox->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    lateralDistanceBox->setToolTip(lateralDistanceLab->toolTip());
    lateralDistanceBox->setDecimals(4);
    lateralDistanceBox->setValue(0.0885);
    lateralDistanceBox->setSingleStep(0.01);
    lateralDistanceBox->setMinimum(0);
    lateralDistanceBox->setMaximum(5);
    lateralDistanceLay->addWidget(lateralDistanceBox);
    
    swingSettingsLayout->addLayout(lateralDistanceLay);
    
    QGroupBox* swingSettingsBox = new QGroupBox;
    swingSettingsBox->setTitle("Swing Foot Settings");
    swingSettingsBox->setStyleSheet(groupStyleSheet);
    swingSettingsBox->setLayout(swingSettingsLayout);
    rightColumn->addWidget(swingSettingsBox);
    
    
    
    QVBoxLayout* comSettingsLayout = new QVBoxLayout;
    comSettingsLayout->setAlignment(Qt::AlignCenter);
    
    QHBoxLayout* comHeightLay = new QHBoxLayout;
    QLabel* comHeightLab = new QLabel;
    comHeightLab->setText("Center of Mass Height:");
    comHeightLab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    comHeightLab->setToolTip("How heigh (m) should the center of mass be from the ground?");
    comHeightLay->addWidget(comHeightLab);

    comHeightBox = new QDoubleSpinBox;
    comHeightBox->setSizePolicy(pbsize);
    comHeightBox->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    comHeightBox->setToolTip(comHeightLab->toolTip());
    comHeightBox->setDecimals(3);
    comHeightBox->setValue(0.5);
    comHeightBox->setSingleStep(0.01);
    comHeightBox->setMinimum(0);
    comHeightBox->setMaximum(5);
    comHeightLay->addWidget(comHeightBox);
    
    comSettingsLayout->addLayout(comHeightLay);
    
    QHBoxLayout* comIKAngleWeightLay = new QHBoxLayout;
    QLabel* comIKAngleWeightLab = new QLabel;
    comIKAngleWeightLab->setText("IK Angle Weight:");
    comIKAngleWeightLab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    comIKAngleWeightLab->setToolTip("I don't really know what this does.");
    comIKAngleWeightLay->addWidget(comIKAngleWeightLab);

    comIKAngleWeightBox = new QDoubleSpinBox;
    comIKAngleWeightBox->setSizePolicy(pbsize);
    comIKAngleWeightBox->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    comIKAngleWeightBox->setToolTip(comIKAngleWeightLab->toolTip());
    comIKAngleWeightBox->setDecimals(3);
    comIKAngleWeightBox->setValue(0);
    comIKAngleWeightBox->setSingleStep(0.01);
    comIKAngleWeightBox->setMinimum(0);
    comIKAngleWeightBox->setMaximum(5);
    comIKAngleWeightLay->addWidget(comIKAngleWeightBox);
    
    comSettingsLayout->addLayout(comIKAngleWeightLay);
    
    QGroupBox* comSettingsBox = new QGroupBox;
    comSettingsBox->setTitle("Center of Mass Settings");
    comSettingsBox->setStyleSheet(groupStyleSheet);
    comSettingsBox->setLayout(comSettingsLayout);
    rightColumn->addWidget(comSettingsBox);
    
    
    QHBoxLayout* bottomLayout = new QHBoxLayout;
    bottomLayout->addLayout(leftColumn);
    bottomLayout->addLayout(rightColumn);
    
    QVBoxLayout* masterZMPLayout = new QVBoxLayout;
    masterZMPLayout->addLayout(profileLayoutMaster);
    masterZMPLayout->addLayout(bottomLayout);
    
    zmpParamTab = new QWidget;
    zmpParamTab->setLayout(masterZMPLayout);
    
    saveAsEdit->setText("Default");
    handleProfileSaveAs();
    saveAsEdit->setText("Default-Backup");
    handleProfileSaveAs();
    saveAsEdit->clear();
    
    profileSelect->setCurrentIndex(0);
}
#endif // HAVE_HUBOMZ

void HuboWalkWidget::initializeBalParamTab()
{
    QSizePolicy pbsize(QSizePolicy::Maximum, QSizePolicy::Maximum);

    QHBoxLayout* balProfileLayoutTop = new QHBoxLayout;
    QLabel* balProfileLab = new QLabel;
    balProfileLab->setText("Profile:");
    balProfileLayoutTop->addWidget(balProfileLab, 0, Qt::AlignVCenter | Qt::AlignRight);

    balProfileSelect = new QComboBox;
    balProfileLayoutTop->addWidget(balProfileSelect);
    connect(balProfileSelect, SIGNAL(currentIndexChanged(int)), this, SLOT(handlebalProfileSelect(int)));

    savebalProfile = new QPushButton;
    savebalProfile->setSizePolicy(pbsize);
    savebalProfile->setText("Save");
    savebalProfile->setToolTip("Save the values below into the currently selected profile");
    balProfileLayoutTop->addWidget(savebalProfile);
    connect(savebalProfile, SIGNAL(clicked()), this, SLOT(handlebalProfileSave()));

    deletebalProfile = new QPushButton;
    deletebalProfile->setSizePolicy(pbsize);
    deletebalProfile->setText("Delete");
    deletebalProfile->setToolTip("Remove the current profile from the list\n"
                                 "WARNING: This is permanent!");
    balProfileLayoutTop->addWidget(deletebalProfile);
    connect(deletebalProfile, SIGNAL(clicked()), this, SLOT(handlebalProfileDelete()));

    QHBoxLayout* balProfileLayoutBottom = new QHBoxLayout;
    saveAsbalProfile = new QPushButton;
    saveAsbalProfile->setSizePolicy(pbsize);
    saveAsbalProfile->setText("Save As...");
    saveAsbalProfile->setToolTip("Save the values below as a new profile with the following name:");
    balProfileLayoutBottom->addWidget(saveAsbalProfile);
    connect(saveAsbalProfile, SIGNAL(clicked()), this, SLOT(handlebalProfileSaveAs()));

    balSaveAsEdit = new QLineEdit;
    balSaveAsEdit->setToolTip("Enter a name for a new profile");
    balProfileLayoutBottom->addWidget(balSaveAsEdit);

    QVBoxLayout* balProfileLayoutMaster = new QVBoxLayout;
    balProfileLayoutMaster->addLayout(balProfileLayoutTop);
    balProfileLayoutMaster->addLayout(balProfileLayoutBottom);


    QVBoxLayout* bottomLayout = new QVBoxLayout;

    QHBoxLayout* flatLayout = new QHBoxLayout;
    QLabel* flatLab = new QLabel;
    flatLab->setText("Flattening Gains:");
    flatLab->setToolTip("Gains for foot flattening (Left/Right)");
    flatLayout->addWidget(flatLab);
    flattenBoxL = new QDoubleSpinBox;
    flattenBoxL->setDecimals(4);
    flattenBoxL->setSingleStep(0.003);
    flattenBoxL->setMinimum(0);
    flattenBoxL->setMaximum(99999);
    flattenBoxL->setValue(0.001);
    flatLayout->addWidget(flattenBoxL);
    flattenBoxR = new QDoubleSpinBox;
    flattenBoxR->setDecimals(4);
    flattenBoxR->setSingleStep(0.003);
    flattenBoxR->setMinimum(0);
    flattenBoxR->setMaximum(99999);
    flattenBoxR->setValue(0.001);
    flatLayout->addWidget(flattenBoxR);

    bottomLayout->addLayout(flatLayout);

    QHBoxLayout* decayLayout = new QHBoxLayout;
    QLabel* decayLab = new QLabel;
    decayLab->setText("Decay Gain:");
    decayLayout->addWidget(decayLab);
    decayBox = new QDoubleSpinBox;
    decayBox->setDecimals(4);
    decayBox->setSingleStep(0.05);
    decayBox->setMinimum(0);
    decayBox->setMaximum(1);
    decayBox->setValue(0.5);
    decayLayout->addWidget(decayBox);

    bottomLayout->addLayout(decayLayout);

    QHBoxLayout* threshMinLayout = new QHBoxLayout;
    QLabel* threshMinLab = new QLabel;
    threshMinLab->setText("Minimum Force Threshold:");
    threshMinLab->setToolTip("Minimum force cut-off for flattening foot (Left/Right)");
    threshMinLayout->addWidget(threshMinLab);
    threshMinBoxL = new QDoubleSpinBox;
    threshMinBoxL->setDecimals(4);
    threshMinBoxL->setSingleStep(1);
    threshMinBoxL->setMinimum(-99999);
    threshMinBoxL->setMaximum(99999);
    threshMinBoxL->setValue(10);
    threshMinLayout->addWidget(threshMinBoxL);
    threshMinBoxR = new QDoubleSpinBox;
    threshMinBoxR->setDecimals(4);
    threshMinBoxR->setSingleStep(1);
    threshMinBoxR->setMinimum(-99999);
    threshMinBoxR->setMaximum(99999);
    threshMinBoxR->setValue(10);
    threshMinLayout->addWidget(threshMinBoxR);

    bottomLayout->addLayout(threshMinLayout);

    QHBoxLayout* threshMaxLayout = new QHBoxLayout;
    QLabel* threshMaxLab = new QLabel;
    threshMaxLab->setText("Maximum Force Threshold:");
    threshMaxLab->setToolTip("Minimum force cut-off for flattening foot (Left/Right)");
    threshMaxLayout->addWidget(threshMaxLab);
    threshMaxBoxL = new QDoubleSpinBox;
    threshMaxBoxL->setDecimals(4);
    threshMaxBoxL->setSingleStep(1);
    threshMaxBoxL->setMinimum(-99999);
    threshMaxBoxL->setMaximum(99999);
    threshMaxBoxL->setValue(50);
    threshMaxLayout->addWidget(threshMaxBoxL);
    threshMaxBoxR = new QDoubleSpinBox;
    threshMaxBoxR->setDecimals(4);
    threshMaxBoxR->setSingleStep(1);
    threshMaxBoxR->setMinimum(-99999);
    threshMaxBoxR->setMaximum(99999);
    threshMaxBoxR->setValue(50);
    threshMaxLayout->addWidget(threshMaxBoxR);

    bottomLayout->addLayout(threshMaxLayout);

    QHBoxLayout* straightenPLayout = new QHBoxLayout;
    QLabel* straightenPLab = new QLabel;
    straightenPLab->setText("IMU Offset Gain P:");
    straightenPLab->setToolTip("Gain for keeping torso upright (Front/Back)");
    straightenPLayout->addWidget(straightenPLab);
    straightenPBoxL = new QDoubleSpinBox;
    straightenPBoxL->setDecimals(4);
    straightenPBoxL->setSingleStep(1);
    straightenPBoxL->setMinimum(-99999);
    straightenPBoxL->setMaximum(99999);
    straightenPBoxL->setValue(0.04);
    straightenPLayout->addWidget(straightenPBoxL);
    straightenPBoxR = new QDoubleSpinBox;
    straightenPBoxR->setDecimals(4);
    straightenPBoxR->setSingleStep(1);
    straightenPBoxR->setMinimum(-99999);
    straightenPBoxR->setMaximum(99999);
    straightenPBoxR->setValue(0.012);
    straightenPLayout->addWidget(straightenPBoxR);

    bottomLayout->addLayout(straightenPLayout);

    QHBoxLayout* straightenRLayout = new QHBoxLayout;
    QLabel* straightenRLab = new QLabel;
    straightenRLab->setText("IMU Offset Gain R:");
    straightenRLab->setToolTip("Gain for keeping torso upright (Left/Right)");
    straightenRLayout->addWidget(straightenRLab);
    straightenRBoxL = new QDoubleSpinBox;
    straightenRBoxL->setDecimals(4);
    straightenRBoxL->setSingleStep(1);
    straightenRBoxL->setMinimum(-99999);
    straightenRBoxL->setMaximum(99999);
    straightenRBoxL->setValue(0);
    straightenRLayout->addWidget(straightenRBoxL);
    straightenRBoxR = new QDoubleSpinBox;
    straightenRBoxR->setDecimals(4);
    straightenRBoxR->setSingleStep(1);
    straightenRBoxR->setMinimum(-99999);
    straightenRBoxR->setMaximum(99999);
    straightenRBoxR->setValue(0);
    straightenRLayout->addWidget(straightenRBoxR);

    bottomLayout->addLayout(straightenRLayout);

    QHBoxLayout* springLayout = new QHBoxLayout;
    QLabel* springLab = new QLabel;
    springLab->setText("Squat Velocity Gain:");
    springLab->setToolTip("Gain for the speed of squatting up and down");
    springLayout->addWidget(springLab);
    springBoxL = new QDoubleSpinBox;
    springBoxL->setDecimals(4);
    springBoxL->setSingleStep(1);
    springBoxL->setMinimum(-99999);
    springBoxL->setMaximum(99999);
    springBoxL->setValue(0.2);
    springLayout->addWidget(springBoxL);
    springBoxR = new QDoubleSpinBox;
    springBoxR->setDecimals(4);
    springBoxR->setSingleStep(1);
    springBoxR->setMinimum(-99999);
    springBoxR->setMaximum(99999);
    springBoxR->setValue(0.2);
    springLayout->addWidget(springBoxR);

    bottomLayout->addLayout(springLayout);

    QHBoxLayout* dampLayout = new QHBoxLayout;
    QLabel* dampLab = new QLabel;
    dampLab->setText("Damping Gain:");
    dampLab->setToolTip("Damping gain for complying the knees");
    dampLayout->addWidget(dampLab);
    dampBoxL = new QDoubleSpinBox;
    dampBoxL->setDecimals(4);
    dampBoxL->setSingleStep(1);
    dampBoxL->setMinimum(-99999);
    dampBoxL->setMaximum(99999);
    dampBoxL->setValue(0);
    dampLayout->addWidget(dampBoxL);
    dampBoxR = new QDoubleSpinBox;
    dampBoxR->setDecimals(4);
    dampBoxR->setSingleStep(1);
    dampBoxR->setMinimum(-99999);
    dampBoxR->setMaximum(99999);
    dampBoxR->setValue(0);
    dampLayout->addWidget(dampBoxR);

    bottomLayout->addLayout(dampLayout);

    QHBoxLayout* responseLayout = new QHBoxLayout;
    QLabel* responseLab = new QLabel;
    responseLab->setText("Response Gain:");
    responseLab->setToolTip("Knee response based on Fz in feet");
    responseLayout->addWidget(responseLab);
    responseBoxL = new QDoubleSpinBox;
    responseBoxL->setDecimals(4);
    responseBoxL->setSingleStep(1);
    responseBoxL->setMinimum(-99999);
    responseBoxL->setMaximum(99999);
    responseBoxL->setValue(0);
    responseLayout->addWidget(responseBoxL);
    responseBoxR = new QDoubleSpinBox;
    responseBoxR->setDecimals(4);
    responseBoxR->setSingleStep(1);
    responseBoxR->setMinimum(-99999);
    responseBoxR->setMaximum(99999);
    responseBoxR->setValue(0);
    responseLayout->addWidget(responseBoxR);

    bottomLayout->addLayout(responseLayout);

    // Single-support hip nudge proportional and derivative gains based on ankle torque readings
    QHBoxLayout* singleSupportHipNudgeGainLayout = new QHBoxLayout;
    QLabel* singleSupportHipNudgeGainLab = new QLabel;
    singleSupportHipNudgeGainLab->setText("Single-Support Hip Nudge (Kp, Kd):");
    singleSupportHipNudgeGainLab->setToolTip("Gains for nudging the hips in single-support using feedback from the F/T sensors");
    singleSupportHipNudgeGainLayout->addWidget(singleSupportHipNudgeGainLab);
    singleSupportHipNudgeGainBoxP = new QDoubleSpinBox;
    singleSupportHipNudgeGainBoxP->setDecimals(4);
    singleSupportHipNudgeGainBoxP->setSingleStep(0.01);
    singleSupportHipNudgeGainBoxP->setMinimum(0);
    singleSupportHipNudgeGainBoxP->setMaximum(10);
    singleSupportHipNudgeGainBoxP->setValue(0);
    singleSupportHipNudgeGainLayout->addWidget(singleSupportHipNudgeGainBoxP);
    singleSupportHipNudgeGainBoxD = new QDoubleSpinBox;
    singleSupportHipNudgeGainBoxD->setDecimals(4);
    singleSupportHipNudgeGainBoxD->setSingleStep(0.01);
    singleSupportHipNudgeGainBoxD->setMinimum(0);
    singleSupportHipNudgeGainBoxD->setMaximum(10);
    singleSupportHipNudgeGainBoxD->setValue(0);
    singleSupportHipNudgeGainLayout->addWidget(singleSupportHipNudgeGainBoxD);

    bottomLayout->addLayout(singleSupportHipNudgeGainLayout);

    // Double-support hip nudge proportional and derivative gains based on ankle torque readings.
    QHBoxLayout* doubleSupportHipNudgeGainLayout = new QHBoxLayout;
    QLabel* doubleSupportHipNudgeGainLab = new QLabel;
    doubleSupportHipNudgeGainLab->setText("Double-Support Hip Nudge (Kp, Kd):");
    doubleSupportHipNudgeGainLab->setToolTip("Gains for nudging the hips in double-support using feedback from the F/T sensors");
    doubleSupportHipNudgeGainLayout->addWidget(doubleSupportHipNudgeGainLab);
    doubleSupportHipNudgeGainBoxP = new QDoubleSpinBox;
    doubleSupportHipNudgeGainBoxP->setDecimals(4);
    doubleSupportHipNudgeGainBoxP->setSingleStep(0.01);
    doubleSupportHipNudgeGainBoxP->setMinimum(0);
    doubleSupportHipNudgeGainBoxP->setMaximum(10);
    doubleSupportHipNudgeGainBoxP->setValue(0);
    doubleSupportHipNudgeGainLayout->addWidget(doubleSupportHipNudgeGainBoxP);
    doubleSupportHipNudgeGainBoxD = new QDoubleSpinBox;
    doubleSupportHipNudgeGainBoxD->setDecimals(4);
    doubleSupportHipNudgeGainBoxD->setSingleStep(0.01);
    doubleSupportHipNudgeGainBoxD->setMinimum(0);
    doubleSupportHipNudgeGainBoxD->setMaximum(10);
    doubleSupportHipNudgeGainBoxD->setValue(0);
    doubleSupportHipNudgeGainLayout->addWidget(doubleSupportHipNudgeGainBoxD);

    bottomLayout->addLayout(doubleSupportHipNudgeGainLayout);

    updateBalParams = new QPushButton;
    updateBalParams->setText("Send");
    updateBalParams->setToolTip("Send this set of parameters to the balancing daemon");
    connect(updateBalParams, SIGNAL(clicked()), this, SLOT(sendBalParams()));
    bottomLayout->addWidget(updateBalParams);

    balSaveAsEdit->setText("Default");
    handlebalProfileSaveAs();
    balSaveAsEdit->setText("Default-Backup");
    handlebalProfileSaveAs();
    balSaveAsEdit->clear();

    balProfileSelect->setCurrentIndex(0);

    QVBoxLayout* masterBalLayout = new QVBoxLayout;
    masterBalLayout->addLayout(balProfileLayoutMaster);
    masterBalLayout->addLayout(bottomLayout);

    balParamTab = new QWidget;
    balParamTab->setLayout(masterBalLayout);
}

void HuboWalkWidget::initializeLadderTab()
{
    QSizePolicy pbsize(QSizePolicy::Maximum, QSizePolicy::Maximum);

    QHBoxLayout* ladder_profileLayoutTop = new QHBoxLayout;
    QLabel* ladder_profileLab = new QLabel;
    ladder_profileLab->setText("Profile:");
    ladder_profileLayoutTop->addWidget(ladder_profileLab, 0, Qt::AlignVCenter | Qt::AlignRight);

    ladder_profileSelect = new QComboBox;
    ladder_profileLayoutTop->addWidget(profileSelect);
    connect(ladder_profileSelect, SIGNAL(currentIndexChanged(int)), this, SLOT(handleProfileSelect(int)));

    ladder_saveProfile = new QPushButton;
    ladder_saveProfile->setSizePolicy(pbsize);
    ladder_saveProfile->setText("Save");
    ladder_saveProfile->setToolTip("Save the values below into the currently selected profile");
    ladder_profileLayoutTop->addWidget(ladder_saveProfile);
    connect(ladder_saveProfile, SIGNAL(clicked()), this, SLOT(handleProfileSave()));

    ladder_deleteProfile = new QPushButton;
    ladder_deleteProfile->setSizePolicy(pbsize);
    ladder_deleteProfile->setText("Delete");
    ladder_deleteProfile->setToolTip("Remove the current profile from the list\n"
                              "WARNING: This is permanent!");
    ladder_profileLayoutTop->addWidget(ladder_deleteProfile);
    connect(ladder_deleteProfile, SIGNAL(clicked()), this, SLOT(handleProfileDelete()));

    QHBoxLayout* ladder_profileLayoutBottom = new QHBoxLayout;
    ladder_saveAsProfile = new QPushButton;
    ladder_saveAsProfile->setSizePolicy(pbsize);
    ladder_saveAsProfile->setText("Save As...");
    ladder_saveAsProfile->setToolTip("Save the values below as a new profile with the following name:");
    ladder_profileLayoutBottom->addWidget(ladder_saveAsProfile);
    connect(ladder_saveAsProfile, SIGNAL(clicked()), this, SLOT(handleProfileSaveAs()));
 
    ladder_saveAsEdit = new QLineEdit;
    ladder_saveAsEdit->setToolTip("Enter a name for a new profile");
    ladder_profileLayoutBottom->addWidget(ladder_saveAsEdit);

    QVBoxLayout* ladder_profileLayoutMaster = new QVBoxLayout;
    ladder_profileLayoutMaster->addLayout(ladder_profileLayoutTop);
    ladder_profileLayoutMaster->addLayout(ladder_profileLayoutBottom);

    QVBoxLayout* ladder_leftColumn = new QVBoxLayout;

    QVBoxLayout* ladder_rungSettingsLayout = new QVBoxLayout;
    ladder_rungSettingsLayout->setAlignment(Qt::AlignCenter);

    QHBoxLayout* rung_widthLay = new QHBoxLayout;
    QLabel* rung_widthLab = new QLabel;
    rung_widthLab->setText("Rung width");
    rung_widthLab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    rung_widthLab->setToolTip("Rung widht");
    rung_widthLay->addWidget(rung_widthLab);

    rung_widthBox = new QDoubleSpinBox;
    rung_widthBox->setSizePolicy(pbsize);
    rung_widthBox->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    rung_widthBox->setToolTip(rung_widthLab->toolTip());
    rung_widthBox->setDecimals(4);
    rung_widthBox->setValue(0.038);
    rung_widthBox->setSingleStep(0.01);
    rung_widthBox->setMinimum(0);
    rung_widthBox->setMaximum(10);
    rung_widthLay->addWidget(rung_widthBox);
    ladder_rungSettingsLayout->addLayout(rung_widthLay);
   
    QHBoxLayout* rung_lengthLay = new QHBoxLayout;
    QLabel* rung_lengthLab = new QLabel;
    rung_lengthLab->setText("rung length");
    rung_lengthLab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    rung_lengthLab->setToolTip("Rung length");
    rung_lengthLay->addWidget(rung_lengthLab);

    rung_lengthBox = new QDoubleSpinBox;
    rung_lengthBox->setSizePolicy(pbsize);
    rung_lengthBox->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    rung_lengthBox->setToolTip(rung_lengthLab->toolTip());
    rung_lengthBox->setDecimals(4);
    rung_lengthBox->setValue(0.038);
    rung_lengthBox->setSingleStep(0.01);
    rung_lengthBox->setMinimum(0);
    rung_lengthBox->setMaximum(10);
    rung_lengthLay->addWidget(rung_lengthBox);

    ladder_rungSettingsLayout->addLayout(rung_lengthLay);
 
    QHBoxLayout* rung_1stheightLay = new QHBoxLayout;
    QLabel* rung_1stheightLab = new QLabel;
    rung_1stheightLab->setText("1st rung spacing");
    rung_1stheightLab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    rung_1stheightLab->setToolTip("1st Rung spacing");
    rung_1stheightLay->addWidget(rung_1stheightLab);

    rung_1stheightBox = new QDoubleSpinBox;
    rung_1stheightBox->setSizePolicy(pbsize);
    rung_1stheightBox->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    rung_1stheightBox->setToolTip(rung_1stheightLab->toolTip());
    rung_1stheightBox->setDecimals(4);
    rung_1stheightBox->setValue(0.038);
    rung_1stheightBox->setSingleStep(0.01);
    rung_1stheightBox->setMinimum(0);
    rung_1stheightBox->setMaximum(10);
    rung_1stheightLay->addWidget(rung_1stheightBox);

    ladder_rungSettingsLayout->addLayout(rung_1stheightLay);

   
    QHBoxLayout* rung_heightLay = new QHBoxLayout;
    QLabel* rung_heightLab = new QLabel;
    rung_heightLab->setText("rung spacing");
    rung_heightLab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    rung_heightLab->setToolTip("Rung spacing");
    rung_heightLay->addWidget(rung_heightLab);

    rung_heightBox = new QDoubleSpinBox;
    rung_heightBox->setSizePolicy(pbsize);
    rung_heightBox->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    rung_heightBox->setToolTip(rung_heightLab->toolTip());
    rung_heightBox->setDecimals(4);
    rung_heightBox->setValue(0.038);
    rung_heightBox->setSingleStep(0.01);
    rung_heightBox->setMinimum(0);
    rung_heightBox->setMaximum(10);
    rung_heightLay->addWidget(rung_heightBox);

    ladder_rungSettingsLayout->addLayout(rung_heightLay);

    // The rail parameters
    QVBoxLayout* ladder_railSettingsLayout = new QVBoxLayout;
    ladder_railSettingsLayout->setAlignment(Qt::AlignCenter);

    QHBoxLayout* rail_heightLay = new QHBoxLayout;
    QLabel* rail_heightLab = new QLabel;
    rail_heightLab->setText("Rail height");
    rail_heightLab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    rail_heightLab->setToolTip("Rail height");
    rail_heightLay->addWidget(rail_heightLab);

    rail_heightBox = new QDoubleSpinBox;
    rail_heightBox->setSizePolicy(pbsize);
    rail_heightBox->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    rail_heightBox->setToolTip(rail_heightLab->toolTip());
    rail_heightBox->setDecimals(4);
    rail_heightBox->setValue(0.038);
    rail_heightBox->setSingleStep(0.01);
    rail_heightBox->setMinimum(0);
    rail_heightBox->setMaximum(10);
    rail_heightLay->addWidget(rail_heightBox);

    ladder_railSettingsLayout->addLayout(rail_heightLay);

    QHBoxLayout* rail_radiusLay = new QHBoxLayout;
    QLabel* rail_radiusLab = new QLabel;
    rail_radiusLab->setText("Rail radius");
    rail_radiusLab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    rail_radiusLab->setToolTip("Rail radius");
    rail_radiusLay->addWidget(rail_radiusLab);

    rail_radiusBox = new QDoubleSpinBox;
    rail_radiusBox->setSizePolicy(pbsize);
    rail_radiusBox->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    rail_radiusBox->setToolTip(rail_radiusLab->toolTip());
    rail_radiusBox->setDecimals(4);
    rail_radiusBox->setValue(0.038);
    rail_radiusBox->setSingleStep(0.01);
    rail_radiusBox->setMinimum(0);
    rail_radiusBox->setMaximum(10);
    rail_radiusLay->addWidget(rail_radiusBox);

    ladder_railSettingsLayout->addLayout(rail_radiusLay);

    //Number and angle of stairs
    QVBoxLayout* ladder_stairSettingsLayout = new QVBoxLayout;
    ladder_stairSettingsLayout->setAlignment(Qt::AlignCenter);

    QHBoxLayout* stair_numberLay = new QHBoxLayout;
    QLabel* stair_numberLab = new QLabel;
    stair_numberLab->setText("Number of Stairs");
    stair_numberLab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    stair_numberLab->setToolTip("Number of Stairs");
    stair_numberLay->addWidget(stair_numberLab);

    stair_numberBox = new QDoubleSpinBox;
    stair_numberBox->setSizePolicy(pbsize);
    stair_numberBox->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    stair_numberBox->setToolTip(stair_numberLab->toolTip());
    stair_numberBox->setDecimals(0);
    stair_numberBox->setValue(0.038);
    stair_numberBox->setSingleStep(1);
    stair_numberBox->setMinimum(0);
    stair_numberBox->setMaximum(10);
    stair_numberLay->addWidget(stair_numberBox);

    ladder_stairSettingsLayout->addLayout(stair_numberLay);

    QHBoxLayout* stair_slopeLay = new QHBoxLayout;
    QLabel* stair_slopeLab = new QLabel;
    stair_slopeLab->setText("Slope(Degree) of Stairs");
    stair_slopeLab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    stair_slopeLab->setToolTip("Slope(Degree) of Stairs");
    stair_slopeLay->addWidget(stair_slopeLab);

    stair_slopeBox = new QDoubleSpinBox;
    stair_slopeBox->setSizePolicy(pbsize);
    stair_slopeBox->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    stair_slopeBox->setToolTip(stair_slopeLab->toolTip());
    stair_slopeBox->setDecimals(0);
    stair_slopeBox->setValue(0.038);
    stair_slopeBox->setSingleStep(1);
    stair_slopeBox->setMinimum(0);
    stair_slopeBox->setMaximum(90);
    stair_slopeLay->addWidget(stair_slopeBox);

    ladder_stairSettingsLayout->addLayout(stair_slopeLay);

 
    QHBoxLayout* rail_slopeLay = new QHBoxLayout;
    QLabel* rail_slopeLab = new QLabel;
    rail_slopeLab->setText("Slope(Degree) of Rails");
    rail_slopeLab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    rail_slopeLab->setToolTip("Slope(Degree) of Rails");
    rail_slopeLay->addWidget(rail_slopeLab);

    rail_slopeBox = new QDoubleSpinBox;
    rail_slopeBox->setSizePolicy(pbsize);
    rail_slopeBox->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    rail_slopeBox->setToolTip(stair_slopeLab->toolTip());
    rail_slopeBox->setDecimals(0);
    rail_slopeBox->setValue(0.038);
    rail_slopeBox->setSingleStep(1);
    rail_slopeBox->setMinimum(0);
    rail_slopeBox->setMaximum(90);
    rail_slopeLay->addWidget(rail_slopeBox);

    ladder_stairSettingsLayout->addLayout(rail_slopeLay);


    QHBoxLayout* rail_startLay = new QHBoxLayout;
    QLabel* rail_startLab = new QLabel;
    rail_startLab->setText("Start of the rail");
    rail_startLab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    rail_startLab->setToolTip("Start of the rail");
    rail_startLay->addWidget(rail_startLab);

    rail_startBox = new QDoubleSpinBox;
    rail_startBox->setSizePolicy(pbsize);
    rail_startBox->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    rail_startBox->setToolTip(rail_startLab->toolTip());
    rail_startBox->setDecimals(4);
    rail_startBox->setValue(0.038);
    rail_startBox->setSingleStep(1);
    rail_startBox->setMinimum(0);
    rail_startBox->setMaximum(90);
    rail_startLay->addWidget(rail_startBox);

    ladder_stairSettingsLayout->addLayout(rail_startLay);

    QHBoxLayout* rail_endLay = new QHBoxLayout;
    QLabel* rail_endLab = new QLabel;
    rail_endLab->setText("Start of the rail");
    rail_endLab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    rail_endLab->setToolTip("Start of the rail");
    rail_endLay->addWidget(rail_endLab);

    rail_endBox = new QDoubleSpinBox;
    rail_endBox->setSizePolicy(pbsize);
    rail_endBox->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    rail_endBox->setToolTip(rail_endLab->toolTip());
    rail_endBox->setDecimals(4);
    rail_endBox->setValue(0.038);
    rail_endBox->setSingleStep(1);
    rail_endBox->setMinimum(0);
    rail_endBox->setMaximum(90);
    rail_endLay->addWidget(rail_endBox);

    ladder_stairSettingsLayout->addLayout(rail_endLay);

   //Plan button
    QVBoxLayout* ladder_sendSettingsLayout = new QVBoxLayout;
    ladder_sendSettingsLayout->setAlignment(Qt::AlignCenter);

    ladder_sendButton = new QPushButton;
    ladder_sendButton->setText("  Plan motion ");
    ladder_sendButton->setSizePolicy(pbsize);
    ladder_sendSettingsLayout->addWidget(ladder_sendButton, 1, Qt::AlignCenter);
    connect(ladder_sendButton, SIGNAL(clicked()), this, SLOT(handleLadderSend()));
 

   //Run button
    QVBoxLayout* ladder_runSettingsLayout = new QVBoxLayout;
    ladder_runSettingsLayout->setAlignment(Qt::AlignCenter);

    ladder_runButton = new QPushButton;
    ladder_runButton->setText(" Run motion ");
    ladder_runButton->setSizePolicy(pbsize);
    ladder_runSettingsLayout->addWidget(ladder_runButton, 1, Qt::AlignCenter);
    connect(ladder_runButton, SIGNAL(clicked()), this, SLOT(handleLadderRun()));
 

    // master Layout
    QVBoxLayout* masterLadderLayout = new QVBoxLayout;
    masterLadderLayout->addLayout(ladder_profileLayoutTop);
    masterLadderLayout->addLayout(ladder_rungSettingsLayout);
    masterLadderLayout->addLayout(ladder_railSettingsLayout);
    masterLadderLayout->addLayout(ladder_stairSettingsLayout);
    masterLadderLayout->addLayout(ladder_sendSettingsLayout);
    masterLadderLayout->addLayout(ladder_runSettingsLayout);


    ladderTab = new QWidget;
    ladderTab->setLayout(masterLadderLayout);
    
    saveAsEdit->setText("Default");
    handleProfileSaveAs();
    saveAsEdit->setText("Default-Backup");
    handleProfileSaveAs();
    saveAsEdit->clear();
    
    profileSelect->setCurrentIndex(0);

}

void HuboWalkWidget::initializeCorrectionTab()
{
    QSizePolicy pbsize(QSizePolicy::Maximum, QSizePolicy::Maximum);

    QHBoxLayout* correction_profileLayoutTop = new QHBoxLayout;
    QLabel* correction_profileLab = new QLabel;
    correction_profileLab->setText("Profile:");
    correction_profileLayoutTop->addWidget(correction_profileLab, 0, Qt::AlignVCenter | Qt::AlignRight);

    correction_profileSelect = new QComboBox;
    correction_profileLayoutTop->addWidget(profileSelect);
    connect(correction_profileSelect, SIGNAL(currentIndexChanged(int)), this, SLOT(handleProfileSelect(int)));

    correction_saveProfile = new QPushButton;
    correction_saveProfile->setSizePolicy(pbsize);
    correction_saveProfile->setText("Save");
    correction_saveProfile->setToolTip("Save the values below into the currently selected profile");
    correction_profileLayoutTop->addWidget(correction_saveProfile);
    connect(correction_saveProfile, SIGNAL(clicked()), this, SLOT(handleProfileSave()));

    correction_deleteProfile = new QPushButton;
    correction_deleteProfile->setSizePolicy(pbsize);
    correction_deleteProfile->setText("Delete");
    correction_deleteProfile->setToolTip("Remove the current profile from the list\n"
                              "WARNING: This is permanent!");
    correction_profileLayoutTop->addWidget(correction_deleteProfile);
    connect(correction_deleteProfile, SIGNAL(clicked()), this, SLOT(handleProfileDelete()));

    QHBoxLayout* correction_profileLayoutBottom = new QHBoxLayout;
    correction_saveAsProfile = new QPushButton;
    correction_saveAsProfile->setSizePolicy(pbsize);
    correction_saveAsProfile->setText("Save As...");
    correction_saveAsProfile->setToolTip("Save the values below as a new profile with the following name:");
    correction_profileLayoutBottom->addWidget(correction_saveAsProfile);
    connect(correction_saveAsProfile, SIGNAL(clicked()), this, SLOT(handleProfileSaveAs()));
 
    correction_saveAsEdit = new QLineEdit;
    correction_saveAsEdit->setToolTip("Enter a name for a new profile");
    correction_profileLayoutBottom->addWidget(correction_saveAsEdit);

    QVBoxLayout* correction_profileLayoutMaster = new QVBoxLayout;
    correction_profileLayoutMaster->addLayout(correction_profileLayoutTop);
    correction_profileLayoutMaster->addLayout(correction_profileLayoutBottom);

    QVBoxLayout* correction_leftColumn = new QVBoxLayout;

    QVBoxLayout* correction_SettingsLayout = new QVBoxLayout;
    correction_SettingsLayout->setAlignment(Qt::AlignCenter);

    //LEFT HAND
    QHBoxLayout* leftHand_xLay = new QHBoxLayout;
    QLabel* leftHand_xLab = new QLabel;
    leftHand_xLab->setText("left_hand x");
    leftHand_xLab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    leftHand_xLab->setToolTip("left_hand x");
    leftHand_xLay->addWidget(leftHand_xLab);

    leftHand_xBox = new QDoubleSpinBox;
    leftHand_xBox->setSizePolicy(pbsize);
    leftHand_xBox->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    leftHand_xBox->setToolTip(leftHand_xLab->toolTip());
    leftHand_xBox->setDecimals(4);
    leftHand_xBox->setValue(0.038);
    leftHand_xBox->setSingleStep(0.01);
    leftHand_xBox->setMinimum(0);
    leftHand_xBox->setMaximum(10);
    leftHand_xLay->addWidget(leftHand_xBox);
    correction_SettingsLayout->addLayout(leftHand_xLay);
    
    QHBoxLayout* leftHand_yLay = new QHBoxLayout;
    QLabel* leftHand_yLab = new QLabel;
    leftHand_yLab->setText("left_hand y");
    leftHand_yLab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    leftHand_yLab->setToolTip("left_hand y");
    leftHand_yLay->addWidget(leftHand_yLab);

    leftHand_yBox = new QDoubleSpinBox;
    leftHand_yBox->setSizePolicy(pbsize);
    leftHand_yBox->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    leftHand_yBox->setToolTip(leftHand_yLab->toolTip());
    leftHand_yBox->setDecimals(4);
    leftHand_yBox->setValue(0.038);
    leftHand_yBox->setSingleStep(0.01);
    leftHand_yBox->setMinimum(0);
    leftHand_yBox->setMaximum(10);
    leftHand_yLay->addWidget(leftHand_yBox);
    correction_SettingsLayout->addLayout(leftHand_yLay);

    QHBoxLayout* leftHand_zLay = new QHBoxLayout;
    QLabel* leftHand_zLab = new QLabel;
    leftHand_zLab->setText("left_hand z");
    leftHand_zLab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    leftHand_zLab->setToolTip("left_hand z");
    leftHand_zLay->addWidget(leftHand_zLab);

    leftHand_zBox = new QDoubleSpinBox;
    leftHand_zBox->setSizePolicy(pbsize);
    leftHand_zBox->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    leftHand_zBox->setToolTip(leftHand_zLab->toolTip());
    leftHand_zBox->setDecimals(4);
    leftHand_zBox->setValue(0.038);
    leftHand_zBox->setSingleStep(0.01);
    leftHand_zBox->setMinimum(0);
    leftHand_zBox->setMaximum(10);
    leftHand_zLay->addWidget(leftHand_zBox);
    correction_SettingsLayout->addLayout(leftHand_zLay);

    QHBoxLayout* leftHand_rollLay = new QHBoxLayout;
    QLabel* leftHand_rollLab = new QLabel;
    leftHand_rollLab->setText("left_hand roll");
    leftHand_rollLab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    leftHand_rollLab->setToolTip("left_hand roll");
    leftHand_rollLay->addWidget(leftHand_rollLab);

    leftHand_rollBox = new QDoubleSpinBox;
    leftHand_rollBox->setSizePolicy(pbsize);
    leftHand_rollBox->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    leftHand_rollBox->setToolTip(leftHand_rollLab->toolTip());
    leftHand_rollBox->setDecimals(4);
    leftHand_rollBox->setValue(0.038);
    leftHand_rollBox->setSingleStep(0.01);
    leftHand_rollBox->setMinimum(0);
    leftHand_rollBox->setMaximum(10);
    leftHand_rollLay->addWidget(leftHand_rollBox);
    correction_SettingsLayout->addLayout(leftHand_rollLay);

    QHBoxLayout* leftHand_pitchLay = new QHBoxLayout;
    QLabel* leftHand_pitchLab = new QLabel;
    leftHand_pitchLab->setText("left_hand pitch");
    leftHand_pitchLab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    leftHand_pitchLab->setToolTip("left_hand pitch");
    leftHand_pitchLay->addWidget(leftHand_pitchLab);

    leftHand_pitchBox = new QDoubleSpinBox;
    leftHand_pitchBox->setSizePolicy(pbsize);
    leftHand_pitchBox->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    leftHand_pitchBox->setToolTip(leftHand_pitchLab->toolTip());
    leftHand_pitchBox->setDecimals(4);
    leftHand_pitchBox->setValue(0.038);
    leftHand_pitchBox->setSingleStep(0.01);
    leftHand_pitchBox->setMinimum(0);
    leftHand_pitchBox->setMaximum(10);
    leftHand_pitchLay->addWidget(leftHand_pitchBox);
    correction_SettingsLayout->addLayout(leftHand_pitchLay);

    QHBoxLayout* leftHand_yawLay = new QHBoxLayout;
    QLabel* leftHand_yawLab = new QLabel;
    leftHand_yawLab->setText("left_hand yaw");
    leftHand_yawLab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    leftHand_yawLab->setToolTip("left_hand yaw");
    leftHand_yawLay->addWidget(leftHand_yawLab);

    leftHand_yawBox = new QDoubleSpinBox;
    leftHand_yawBox->setSizePolicy(pbsize);
    leftHand_yawBox->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    leftHand_yawBox->setToolTip(leftHand_yawLab->toolTip());
    leftHand_yawBox->setDecimals(4);
    leftHand_yawBox->setValue(0.038);
    leftHand_yawBox->setSingleStep(0.01);
    leftHand_yawBox->setMinimum(0);
    leftHand_yawBox->setMaximum(10);
    leftHand_yawLay->addWidget(leftHand_yawBox);
    correction_SettingsLayout->addLayout(leftHand_yawLay);

    // RIGHT HAND
    QHBoxLayout* rightHand_xLay = new QHBoxLayout;
    QLabel* rightHand_xLab = new QLabel;
    rightHand_xLab->setText("right_hand x");
    rightHand_xLab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    rightHand_xLab->setToolTip("right_hand x");
    rightHand_xLay->addWidget(rightHand_xLab);

    rightHand_xBox = new QDoubleSpinBox;
    rightHand_xBox->setSizePolicy(pbsize);
    rightHand_xBox->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    rightHand_xBox->setToolTip(rightHand_xLab->toolTip());
    rightHand_xBox->setDecimals(4);
    rightHand_xBox->setValue(0.038);
    rightHand_xBox->setSingleStep(0.01);
    rightHand_xBox->setMinimum(0);
    rightHand_xBox->setMaximum(10);
    rightHand_xLay->addWidget(rightHand_xBox);
    correction_SettingsLayout->addLayout(rightHand_xLay);
    
    QHBoxLayout* rightHand_yLay = new QHBoxLayout;
    QLabel* rightHand_yLab = new QLabel;
    rightHand_yLab->setText("right_hand y");
    rightHand_yLab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    rightHand_yLab->setToolTip("right_hand y");
    rightHand_yLay->addWidget(rightHand_yLab);

    rightHand_yBox = new QDoubleSpinBox;
    rightHand_yBox->setSizePolicy(pbsize);
    rightHand_yBox->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    rightHand_yBox->setToolTip(rightHand_yLab->toolTip());
    rightHand_yBox->setDecimals(4);
    rightHand_yBox->setValue(0.038);
    rightHand_yBox->setSingleStep(0.01);
    rightHand_yBox->setMinimum(0);
    rightHand_yBox->setMaximum(10);
    rightHand_yLay->addWidget(rightHand_yBox);
    correction_SettingsLayout->addLayout(rightHand_yLay);

    QHBoxLayout* rightHand_zLay = new QHBoxLayout;
    QLabel* rightHand_zLab = new QLabel;
    rightHand_zLab->setText("right_hand z");
    rightHand_zLab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    rightHand_zLab->setToolTip("right_hand z");
    rightHand_zLay->addWidget(rightHand_zLab);

    rightHand_zBox = new QDoubleSpinBox;
    rightHand_zBox->setSizePolicy(pbsize);
    rightHand_zBox->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    rightHand_zBox->setToolTip(rightHand_zLab->toolTip());
    rightHand_zBox->setDecimals(4);
    rightHand_zBox->setValue(0.038);
    rightHand_zBox->setSingleStep(0.01);
    rightHand_zBox->setMinimum(0);
    rightHand_zBox->setMaximum(10);
    rightHand_zLay->addWidget(rightHand_zBox);
    correction_SettingsLayout->addLayout(rightHand_zLay);

    QHBoxLayout* rightHand_rollLay = new QHBoxLayout;
    QLabel* rightHand_rollLab = new QLabel;
    rightHand_rollLab->setText("right_hand roll");
    rightHand_rollLab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    rightHand_rollLab->setToolTip("right_hand roll");
    rightHand_rollLay->addWidget(rightHand_rollLab);

    rightHand_rollBox = new QDoubleSpinBox;
    rightHand_rollBox->setSizePolicy(pbsize);
    rightHand_rollBox->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    rightHand_rollBox->setToolTip(rightHand_rollLab->toolTip());
    rightHand_rollBox->setDecimals(4);
    rightHand_rollBox->setValue(0.038);
    rightHand_rollBox->setSingleStep(0.01);
    rightHand_rollBox->setMinimum(0);
    rightHand_rollBox->setMaximum(10);
    rightHand_rollLay->addWidget(rightHand_rollBox);
    correction_SettingsLayout->addLayout(rightHand_rollLay);

    QHBoxLayout* rightHand_pitchLay = new QHBoxLayout;
    QLabel* rightHand_pitchLab = new QLabel;
    rightHand_pitchLab->setText("right_hand pitch");
    rightHand_pitchLab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    rightHand_pitchLab->setToolTip("right_hand pitch");
    rightHand_pitchLay->addWidget(rightHand_pitchLab);

    rightHand_pitchBox = new QDoubleSpinBox;
    rightHand_pitchBox->setSizePolicy(pbsize);
    rightHand_pitchBox->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    rightHand_pitchBox->setToolTip(rightHand_pitchLab->toolTip());
    rightHand_pitchBox->setDecimals(4);
    rightHand_pitchBox->setValue(0.038);
    rightHand_pitchBox->setSingleStep(0.01);
    rightHand_pitchBox->setMinimum(0);
    rightHand_pitchBox->setMaximum(10);
    rightHand_pitchLay->addWidget(rightHand_pitchBox);
    correction_SettingsLayout->addLayout(rightHand_pitchLay);

    QHBoxLayout* rightHand_yawLay = new QHBoxLayout;
    QLabel* rightHand_yawLab = new QLabel;
    rightHand_yawLab->setText("right_hand yaw");
    rightHand_yawLab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    rightHand_yawLab->setToolTip("right_hand yaw");
    rightHand_yawLay->addWidget(rightHand_yawLab);

    rightHand_yawBox = new QDoubleSpinBox;
    rightHand_yawBox->setSizePolicy(pbsize);
    rightHand_yawBox->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    rightHand_yawBox->setToolTip(rightHand_yawLab->toolTip());
    rightHand_yawBox->setDecimals(4);
    rightHand_yawBox->setValue(0.038);
    rightHand_yawBox->setSingleStep(0.01);
    rightHand_yawBox->setMinimum(0);
    rightHand_yawBox->setMaximum(10);
    rightHand_yawLay->addWidget(rightHand_yawBox);
    correction_SettingsLayout->addLayout(rightHand_yawLay);

   //LEGS
    QHBoxLayout* legs_xLay = new QHBoxLayout;
    QLabel* legs_xLab = new QLabel;
    legs_xLab->setText("legs x");
    legs_xLab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    legs_xLab->setToolTip("legs x");
    legs_xLay->addWidget(legs_xLab);

    legs_xBox = new QDoubleSpinBox;
    legs_xBox->setSizePolicy(pbsize);
    legs_xBox->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    legs_xBox->setToolTip(legs_xLab->toolTip());
    legs_xBox->setDecimals(4);
    legs_xBox->setValue(0.038);
    legs_xBox->setSingleStep(0.01);
    legs_xBox->setMinimum(0);
    legs_xBox->setMaximum(10);
    legs_xLay->addWidget(legs_xBox);
    correction_SettingsLayout->addLayout(legs_xLay);
    
    QHBoxLayout* legs_yLay = new QHBoxLayout;
    QLabel* legs_yLab = new QLabel;
    legs_yLab->setText("legs y");
    legs_yLab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    legs_yLab->setToolTip("legs y");
    legs_yLay->addWidget(legs_yLab);

    legs_yBox = new QDoubleSpinBox;
    legs_yBox->setSizePolicy(pbsize);
    legs_yBox->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    legs_yBox->setToolTip(legs_yLab->toolTip());
    legs_yBox->setDecimals(4);
    legs_yBox->setValue(0.038);
    legs_yBox->setSingleStep(0.01);
    legs_yBox->setMinimum(0);
    legs_yBox->setMaximum(10);
    legs_yLay->addWidget(legs_yBox);
    correction_SettingsLayout->addLayout(legs_yLay);

    QHBoxLayout* legs_zLay = new QHBoxLayout;
    QLabel* legs_zLab = new QLabel;
    legs_zLab->setText("legs z");
    legs_zLab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    legs_zLab->setToolTip("legs z");
    legs_zLay->addWidget(legs_zLab);

    legs_zBox = new QDoubleSpinBox;
    legs_zBox->setSizePolicy(pbsize);
    legs_zBox->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    legs_zBox->setToolTip(legs_zLab->toolTip());
    legs_zBox->setDecimals(4);
    legs_zBox->setValue(0.038);
    legs_zBox->setSingleStep(0.01);
    legs_zBox->setMinimum(0);
    legs_zBox->setMaximum(10);
    legs_zLay->addWidget(legs_zBox);
    correction_SettingsLayout->addLayout(legs_zLay);

    QHBoxLayout* legs_yawLay = new QHBoxLayout;
    QLabel* legs_yawLab = new QLabel;
    legs_yawLab->setText("legs yaw");
    legs_yawLab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    legs_yawLab->setToolTip("legs yaw");
    legs_yawLay->addWidget(legs_yawLab);

    legs_yawBox = new QDoubleSpinBox;
    legs_yawBox->setSizePolicy(pbsize);
    legs_yawBox->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    legs_yawBox->setToolTip(legs_yawLab->toolTip());
    legs_yawBox->setDecimals(4);
    legs_yawBox->setValue(0.038);
    legs_yawBox->setSingleStep(0.01);
    legs_yawBox->setMinimum(0);
    legs_yawBox->setMaximum(10);
    legs_yawLay->addWidget(legs_yawBox);
    correction_SettingsLayout->addLayout(legs_yawLay);

   //Plan button
    QVBoxLayout* correction_sendSettingsLayout = new QVBoxLayout;
    correction_sendSettingsLayout->setAlignment(Qt::AlignCenter);

    correction_sendButton = new QPushButton;
    correction_sendButton->setText("  Plan Correction ");
    correction_sendButton->setSizePolicy(pbsize);
    correction_sendSettingsLayout->addWidget(correction_sendButton, 1, Qt::AlignCenter);
    connect(correction_sendButton, SIGNAL(clicked()), this, SLOT(handleCorrectionSend()));
 

   //Run button
    QVBoxLayout* correction_runSettingsLayout = new QVBoxLayout;
    correction_runSettingsLayout->setAlignment(Qt::AlignCenter);

    correction_runButton = new QPushButton;
    correction_runButton->setText(" Run Correctin ");
    correction_runButton->setSizePolicy(pbsize);
    correction_runSettingsLayout->addWidget(correction_runButton, 1, Qt::AlignCenter);
    connect(correction_runButton, SIGNAL(clicked()), this, SLOT(handleCorrectionRun()));
 

    // master Layout
    QVBoxLayout* masterLadderLayout = new QVBoxLayout;
    masterLadderLayout->addLayout(correction_profileLayoutTop);
    masterLadderLayout->addLayout(correction_SettingsLayout);
    masterLadderLayout->addLayout(correction_sendSettingsLayout);
    masterLadderLayout->addLayout(correction_runSettingsLayout);



    correctionTab = new QWidget;
    correctionTab->setLayout(masterLadderLayout);
    
    saveAsEdit->setText("Default");
    handleProfileSaveAs();
    saveAsEdit->setText("Default-Backup");
    handleProfileSaveAs();
    saveAsEdit->clear();
    
    profileSelect->setCurrentIndex(0);

}

////////////////////////////////////
//Trajectory Follower Tab
void HuboWalkWidget::initializeTrajectoryFollowerTab()
{
    QSizePolicy pbsize(QSizePolicy::Maximum, QSizePolicy::Maximum);
    //File select
    QHBoxLayout* fileLayoutTop = new QHBoxLayout;
    QLabel* fileLab = new QLabel;
    fileLab->setText("File Name");
    fileLayoutTop->addWidget(fileLab, 0, Qt::AlignVCenter | Qt::AlignRight);
    
    //To create a text box
    fileSelect = new QComboBox;
    fileLayoutTop->addWidget(fileSelect);
    connect(fileSelect, SIGNAL(currentIndexChanged(int)), this, SLOT(handlefileSelect(int))); 
    
    openFile = new QPushButton;
    openFile->setSizePolicy(pbsize);
    openFile->setText("Browse");
    openFile->setToolTip("Open the selected file");
    
    fileLayoutTop->addWidget(openFile);
    connect(openFile, SIGNAL(clicked()), this, SLOT(handleopenFile()));

    //Creates a text box
    QHBoxLayout* fileLayoutBottom = new QHBoxLayout;
    fileName = new QLineEdit;
    fileName->setToolTip("Enter the file name");
    fileLayoutBottom->addWidget(fileName);

    // Compliance ON or OFF
    
    QHBoxLayout* complianceSelectLayout = new QHBoxLayout;
    compSelectGroup = new QButtonGroup;
    compSelectGroup->setExclusive(true);
    
    onButton = new QRadioButton;
    onButton->setText("ON");
    onButton->setToolTip("Turn Compliance ON");
    compSelectGroup->addButton(onButton);
    complianceSelectLayout->addWidget(onButton);
    //onButton->setChecked(true);
    
    offButton = new QRadioButton;
    offButton->setText("OFF");
    offButton->setToolTip("Turn Compliance OFF");
    compSelectGroup->addButton(offButton);
    complianceSelectLayout->addWidget(offButton);

    changeButton = new QPushButton;
    changeButton->setText("Change");
    changeButton->setToolTip("Change compliance");
    compSelectGroup->addButton(changeButton);
    complianceSelectLayout->addWidget(changeButton);
    
    QGroupBox* complianceSelectBox = new QGroupBox;
    complianceSelectBox->setTitle("Compliance Control");
    complianceSelectBox->setStyleSheet(groupStyleSheet);
    complianceSelectBox->setLayout(complianceSelectLayout);
    complianceSelectBox->setAlignment(Qt::AlignAbsolute);
    //pauseStatusLayout->addWidget(complianceSelectBox);

    //Run button
    QHBoxLayout* runLayout=new QHBoxLayout;
    runButton=new QPushButton;
    runButton->setSizePolicy(pbsize);
    runButton->setText("Run");
    runButton->setToolTip("Run the program");
    runLayout->addWidget(runButton);
    runLayout->setAlignment(Qt::AlignLeft);
    connect(runButton, SIGNAL(clicked()), this, SLOT(handleFileRun()));
    
    //Pause Button
    //QHBoxLayout* pauseLayout = new QHBoxLayout;
    pauseButton = new QPushButton;
    pauseButton->setSizePolicy(pbsize);
    pauseButton->setText("Pause");
    pauseButton->setToolTip("Pause the execution");
    runLayout->addWidget(pauseButton);
    connect(pauseButton, SIGNAL(clicked()), this, SLOT(handleFilePause()));
    // Will always show paused (if pauseButton==1) then show paused?

    QHBoxLayout* pauseStatusLayout = new QHBoxLayout;
    QLabel* pauselab = new QLabel;
    pauselab->setText("Status:");
    pauseStatusLayout->addWidget(pauselab);
    pauseStatus = new QLabel;
    pauseStatus->setText("Paused");
    pauseStatusLayout->addWidget(pauseStatus);
    pauseStatusLayout->setAlignment(Qt::AlignHCenter);
    //pauseLayout->addLayout(pauseStatusLayout);

    // Display to show frequence=200Hz
    QHBoxLayout* freqDisplayLayout = new QHBoxLayout;
    QLabel* freqDisplaylab = new QLabel;
    freqDisplaylab->setText("Frequency");
    freqDisplayLayout->addWidget(freqDisplaylab);
    QLabel* freqValue = new QLabel;
    freqValue->setText("200 Hz");
    freqDisplayLayout->addWidget(freqValue);
    freqDisplayLayout->setAlignment(Qt::AlignAbsolute);
    
    pauseStatusLayout->addLayout(freqDisplayLayout);
    pauseStatusLayout->addWidget(complianceSelectBox);
    ////////////////////
    
    
    //Master Layout
    QVBoxLayout* masterTFTLayout = new QVBoxLayout;
    masterTFTLayout->addLayout(fileLayoutTop);
    masterTFTLayout->addLayout(fileLayoutBottom);
    masterTFTLayout->addLayout(runLayout);
    masterTFTLayout->addLayout(pauseStatusLayout);
    masterTFTLayout->addLayout(freqDisplayLayout);
    masterTFTLayout->addWidget(complianceSelectBox);
    

    TrajectoryFollowerTab=new QWidget;
    TrajectoryFollowerTab->setLayout(masterTFTLayout);
        
} 
///////////////////////////////////////////////////




}

#include <pluginlib/class_list_macros.h>
PLUGINLIB_EXPORT_CLASS( hubo_walk_space::HuboWalkPanel,rviz::Panel )
PLUGINLIB_EXPORT_CLASS( hubo_walk_space::HuboWalkWidget, QTabWidget )
