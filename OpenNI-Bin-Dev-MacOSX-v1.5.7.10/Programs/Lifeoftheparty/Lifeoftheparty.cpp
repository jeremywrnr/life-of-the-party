
/******************************************************************************
___      ___   _______  _______    _______  _______
|   |    |   | |       ||       |  |       ||       |
|   |    |   | |    ___||    ___|  |   _   ||    ___|
|   |    |   | |   |___ |   |___   |  | |  ||   |___
|   |___ |   | |    ___||    ___|  |  |_|  ||    ___|
|       ||   | |   |    |   |___   |       ||   |
|_______||___| |___|    |_______|  |_______||___|
_______  __   __  _______    _______  _______  ______    _______  __   __
|       ||  | |  ||       |  |       ||   _   ||    _ |  |       ||  | |  |
|_     _||  |_|  ||    ___|  |    _  ||  |_|  ||   | ||  |_     _||  |_|  |
  |   |  |       ||   |___   |   |_| ||       ||   |_||_   |   |  |       |
  |   |  |       ||    ___|  |    ___||       ||    __  |  |   |  |_     _|
  |   |  |   _   ||   |___   |   |    |   _   ||   |  | |  |   |    |   |
  |___|  |__| |__||_______|  |___|    |__| |__||___|  |_|  |___|    |___|

 *                                                                            *
 *  LifeoftheParty beta                                                       *
 *  Copyright (C) 2014 Justin Fraumeni Jeremy Warner Mat Johnson Amelia Keller*
 *                                                                            *
 *  Life of the Party is a motion based light control system that uses the    *
 *  OpenNI SDK to interface with the Kinect for Xbox. This file was originally*
 *  provided with OpenNI as SimpleSkeleton.cpp as a basic example of joint    *
 *  tracking with OpenNI and has been heavily modified. Modifications are     *
 *  noted where they occur in the source. In accordance with Apache 2.0, the  *
 *  original licensce has been left intact below.                             *
 *                                                                            *                                 
 *                                                                            *
 *                                                                            *

******************************************************************************
*****************************************************************************
*                                                                            *
*  OpenNI 1.x Alpha                                                          *
*  Copyright (C) 2012 PrimeSense Ltd.                                        *
*                                                                            *
*  This file is part of OpenNI.                                              *
*                                                                            *
*  Licensed under the Apache License, Version 2.0 (the "License");           *
*  you may not use this file except in compliance with the License.          *
*  You may obtain a copy of the License at                                   *
*                                                                            *
*      http://www.apache.org/licenses/LICENSE-2.0                            *
*                                                                            *
*  Unless required by applicable law or agreed to in writing, software       *
*  distributed under the License is distributed on an "AS IS" BASIS,         *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
*  See the License for the specific language governing permissions and       *
*  limitations under the License.                                            *
*                                                                            *
*****************************************************************************/
//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include <XnCppWrapper.h>
#include <math.h>

//---------------------------------------------------------------------------
// Defines
//---------------------------------------------------------------------------
#define SAMPLE_XML_PATH "../../Config/SamplesConfig.xml"
#define SAMPLE_XML_PATH_LOCAL "SamplesConfig.xml"

//---------------------------------------------------------------------------
// Globals
//---------------------------------------------------------------------------
xn::Context g_Context;
xn::ScriptNode g_scriptNode;
xn::UserGenerator g_UserGenerator;

XnBool g_bNeedPose = FALSE;
XnChar g_strPose[20] = "";

#define MAX_NUM_USERS 15
//---------------------------------------------------------------------------
// Code
//---------------------------------------------------------------------------


XnBool fileExists(const char *fn)
{
	XnBool exists;
	xnOSDoesFileExist(fn, &exists);
	return exists;
}



// Callback: New user was detected
void XN_CALLBACK_TYPE User_NewUser(xn::UserGenerator& /*generator*/, XnUserID nId, void* /*pCookie*/)
{
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    printf("%d New User %d\n", epochTime, nId);
    // New user found
    if (g_bNeedPose)
    {
        g_UserGenerator.GetPoseDetectionCap().StartPoseDetection(g_strPose, nId);
    }
    else
    {
        g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
    }
}




// Callback: An existing user was lost
void XN_CALLBACK_TYPE User_LostUser(xn::UserGenerator& /*generator*/, XnUserID nId, void* /*pCookie*/)
{
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    printf("%d Lost user %d\n", epochTime, nId);
}



// Callback: Detected a pose
void XN_CALLBACK_TYPE UserPose_PoseDetected(xn::PoseDetectionCapability& /*capability*/, const XnChar* strPose, XnUserID nId, void* /*pCookie*/)
{
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    printf("%d Pose %s detected for user %d\n", epochTime, strPose, nId);
    g_UserGenerator.GetPoseDetectionCap().StopPoseDetection(nId);
    g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
}



// Callback: Started calibration
void XN_CALLBACK_TYPE UserCalibration_CalibrationStart(xn::SkeletonCapability& /*capability*/, XnUserID nId, void* /*pCookie*/)
{
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    printf("%d Calibration started for user %d\n", epochTime, nId);
}



void XN_CALLBACK_TYPE UserCalibration_CalibrationComplete(xn::SkeletonCapability& /*capability*/, XnUserID nId, XnCalibrationStatus eStatus, void* /*pCookie*/)
{
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    if (eStatus == XN_CALIBRATION_STATUS_OK)
    {
        // Calibration succeeded
        printf("%d Calibration complete, start tracking user %d\n", epochTime, nId);
        g_UserGenerator.GetSkeletonCap().StartTracking(nId);
    }
    else
    {
        // Calibration failed
        printf("%d Calibration failed for user %d\n", epochTime, nId);
        if(eStatus==XN_CALIBRATION_STATUS_MANUAL_ABORT)
        {
            printf("Manual abort occured, stop attempting to calibrate!");
            return;
        }
        if (g_bNeedPose)
        {
            g_UserGenerator.GetPoseDetectionCap().StartPoseDetection(g_strPose, nId);
        }
        else
        {
            g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
        }
    }
}




#define CHECK_RC(nRetVal, what)					    \
if (nRetVal != XN_STATUS_OK)				    \
{								    \
printf("%s failed: %s\n", what, xnGetStatusString(nRetVal));    \
return nRetVal;						    \
}







int main()
{
    XnStatus nRetVal = XN_STATUS_OK;
    xn::EnumerationErrors errors;

    const char *fn = NULL;
    if    (fileExists(SAMPLE_XML_PATH)) fn = SAMPLE_XML_PATH;
    else if (fileExists(SAMPLE_XML_PATH_LOCAL)) fn = SAMPLE_XML_PATH_LOCAL;
    else {
        printf("Could not find '%s' nor '%s'. Aborting.\n" , SAMPLE_XML_PATH, SAMPLE_XML_PATH_LOCAL);
        return XN_STATUS_ERROR;
    }
    printf("Reading config from: '%s'\n", fn);

    nRetVal = g_Context.InitFromXmlFile(fn, g_scriptNode, &errors);
    if (nRetVal == XN_STATUS_NO_NODE_PRESENT)
    {
        XnChar strError[1024];
        errors.ToString(strError, 1024);
        printf("%s\n", strError);
        return (nRetVal);
    }
    else if (nRetVal != XN_STATUS_OK)
    {
        printf("Open failed: %s\n", xnGetStatusString(nRetVal));
        return (nRetVal);
    }

    nRetVal = g_Context.FindExistingNode(XN_NODE_TYPE_USER, g_UserGenerator);
    if (nRetVal != XN_STATUS_OK)
    {
        nRetVal = g_UserGenerator.Create(g_Context);
        CHECK_RC(nRetVal, "Find user generator");
    }

    XnCallbackHandle hUserCallbacks, hCalibrationStart, hCalibrationComplete, hPoseDetected;
    if (!g_UserGenerator.IsCapabilitySupported(XN_CAPABILITY_SKELETON))
    {
        printf("Supplied user generator doesn't support skeleton\n");
        return 1;
    }
    nRetVal = g_UserGenerator.RegisterUserCallbacks(User_NewUser, User_LostUser, NULL, hUserCallbacks);
    CHECK_RC(nRetVal, "Register to user callbacks");
    nRetVal = g_UserGenerator.GetSkeletonCap().RegisterToCalibrationStart(UserCalibration_CalibrationStart, NULL, hCalibrationStart);
    CHECK_RC(nRetVal, "Register to calibration start");
    nRetVal = g_UserGenerator.GetSkeletonCap().RegisterToCalibrationComplete(UserCalibration_CalibrationComplete, NULL, hCalibrationComplete);
    CHECK_RC(nRetVal, "Register to calibration complete");

    if (g_UserGenerator.GetSkeletonCap().NeedPoseForCalibration())
    {
        g_bNeedPose = TRUE;
        if (!g_UserGenerator.IsCapabilitySupported(XN_CAPABILITY_POSE_DETECTION))
        {
            printf("Pose required, but not supported\n");
            return 1;
        }
        nRetVal = g_UserGenerator.GetPoseDetectionCap().RegisterToPoseDetected(UserPose_PoseDetected, NULL, hPoseDetected);
        CHECK_RC(nRetVal, "Register to Pose Detected");
        g_UserGenerator.GetSkeletonCap().GetCalibrationPose(g_strPose);
    }

    g_UserGenerator.GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_ALL);

    nRetVal = g_Context.StartGeneratingAll();
    CHECK_RC(nRetVal, "StartGenerating");
    
//LIFE OF THE PARTY CODE EDITS BEGIN HERE****************************************
//*******************************************************************************
//********************************************************************************
    
    XnUserID aUsers[MAX_NUM_USERS];
    XnUInt16 nUsers;
    XnSkeletonJointTransformation head;
    XnSkeletonJointTransformation torso;
    XnSkeletonJointTransformation lfoot;
    XnSkeletonJointTransformation rfoot;
    XnSkeletonJointTransformation lhand;
    XnSkeletonJointTransformation rhand;
    
    double maxX = 1;
    double maxY = 1;
    double maxZ = 1;
    char command[200];
    


    printf("Starting to run\n");
    if(g_bNeedPose)
    {
        printf("Assume calibration pose\n");
    }

	while (!xnOSWasKeyboardHit())
    {
        g_Context.WaitOneUpdateAll(g_UserGenerator);
        // print the torso information for the first user already tracking
        nUsers=MAX_NUM_USERS;
        g_UserGenerator.GetUsers(aUsers, nUsers);

        for(XnUInt16 i=0; i<nUsers; i++)
        {
            if(g_UserGenerator.GetSkeletonCap().IsTracking(aUsers[i])==FALSE)
                continue;
            printf("FRAME\n");
        /*    g_UserGenerator.GetSkeletonCap().GetSkeletonJoint(aUsers[i],XN_SKEL_HEAD,head);
                printf("user %d: Head at (%6.2f,%6.2f,%6.2f)\n",aUsers[i],
                                                                head.position.position.X,
                                                                head.position.position.Y,
                                                                head.position.position.Z);
           */
            
            g_UserGenerator.GetSkeletonCap().GetSkeletonJoint(aUsers[i],XN_SKEL_RIGHT_HAND, rhand);
            
            
            
            
            if(abs(rhand.position.position.X) > maxX) maxX = abs(rhand.position.position.X);
            if(abs(rhand.position.position.Y) > maxY) maxY = abs(rhand.position.position.Y);
            if(abs(rhand.position.position.Z) > maxZ)  maxZ  = abs(rhand.position.position.Z);
        
            sprintf(command, "python varcolor.py %f %f %f", abs(rhand.position.position.X)/maxX, abs(rhand.position.position.Y)/maxY, abs(rhand.position.position.Z)/maxZ);
            system(command);
            
            printf("user %d: Right Hand at (%6.2f, %6.2f, %6.2f, %6.2f, %6.2f, %6.2f)\n",aUsers[i],
                   rhand.position.position.X,
                   abs(rhand.position.position.X)/maxX,
                   rhand.position.position.Y,
                   abs(rhand.position.position.Y)/maxY,
                   rhand.position.position.Z,
                   abs(rhand.position.position.Z)/maxZ
                   );
            
          
            
       /*     g_UserGenerator.GetSkeletonCap().GetSkeletonJoint(aUsers[i],XN_SKEL_RIGHT_HAND,rhand);
                printf("user %d: Right Hand at (%6.2f,%6.2f,%6.2f)\n",aUsers[i],
                                                                rhand.position.position.X,
                                                                rhand.position.position.Y,
                                                                rhand.position.position.Z);
            
            g_UserGenerator.GetSkeletonCap().GetSkeletonJoint(aUsers[i],XN_SKEL_LEFT_HAND,lhand);
                printf("user %d: Left Hand at (%6.2f,%6.2f,%6.2f)\n",aUsers[i],
                                                                lhand.position.position.X,
                                                                lhand.position.position.Y,
                                                                lhand.position.position.Z);

            g_UserGenerator.GetSkeletonCap().GetSkeletonJoint(aUsers[i],XN_SKEL_LEFT_FOOT,lfoot);
            printf("user %d: Left Foot at (%6.2f,%6.2f,%6.2f)\n",aUsers[i],
                                                                lfoot.position.position.X,
                                                                lfoot.position.position.Y,
                                                                lfoot.position.position.Z);
            
            
            g_UserGenerator.GetSkeletonCap().GetSkeletonJoint(aUsers[i],XN_SKEL_RIGHT_FOOT,rfoot);
            printf("user %d: Right Foot at (%6.2f,%6.2f,%6.2f)\n",aUsers[i],
                                                                rfoot.position.position.X,
                                                                rfoot.position.position.Y,
                                                                rfoot.position.position.Z);
        */
        }
        

        
        
        

    }
    g_scriptNode.Release();
    g_UserGenerator.Release();
    g_Context.Release();

}



























