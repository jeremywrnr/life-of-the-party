/******************************************************************************
  ___      ___   _______  _______    _______  _______
  |   |    |   | |       ||       |  |       ||       |
  |   |    |   | |    ___||    ___|  |   _   ||    ___|
  |   |    |   | |   |___ |   |___   |  | |  ||   |___
  |   |___ |   | |    ___||    ___|  |  |_|  ||    ___|
  |       ||   | |   |    |   |___   |       ||   |
  |_______||___| |___|    |_______|  |_______||___|
  _______ __   __  _______    _______  _______  ______    _______  __   __
  |     ||  | |  ||       |  |       ||   _   ||    _ |  |       ||  | |  |
  |_   _||  |_|  ||    ___|  |    _  ||  |_|  ||   | ||  |_     _||  |_|  |
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
        g_UserGenerator.GetPoseDetectionCap().StartPoseDetection(g_strPose,
                nId);
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
void XN_CALLBACK_TYPE UserPose_PoseDetected(xn::PoseDetectionCapability&
        /*capability*/, const XnChar* strPose, XnUserID nId, void* /*pCookie*/)
{
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    printf("%d Pose %s detected for user %d\n", epochTime, strPose, nId);
    g_UserGenerator.GetPoseDetectionCap().StopPoseDetection(nId);
    g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
}



// Callback: Started calibration
void XN_CALLBACK_TYPE UserCalibration_CalibrationStart(xn::SkeletonCapability&
        /*capability*/, XnUserID nId, void* /*pCookie*/)
{
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    printf("%d Calibration started for user %d\n", epochTime, nId);
}



    void XN_CALLBACK_TYPE
UserCalibration_CalibrationComplete(xn::SkeletonCapability& /*capability*/,
        XnUserID nId, XnCalibrationStatus eStatus, void* /*pCookie*/)
{
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    if (eStatus == XN_CALIBRATION_STATUS_OK)
    {
        // Calibration succeeded
        printf("%d Calibration complete, start tracking user %d\n", epochTime,
                nId);
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
            g_UserGenerator.GetPoseDetectionCap().StartPoseDetection(g_strPose,
                    nId);
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

/////////////////////////////////////
// life of the color initialization

double avg_g = 0;
double avg_b = 0;
double avg_r = 0;
double STEPCONST = 14.0;
int steps, fastestUser, jumpframegap;
bool netpositivev[4];
double vaverage[4][5];
double r = 255;
double g, b = 0;
double lastx[4], lasty[4],lastz[4] = {0,0,0,0};
double distancechange[4], secondchange[4] = {0,0,0,0};

// END life of the color initialization
/////////////////////////////////////

int main()
{
    XnStatus nRetVal = XN_STATUS_OK;
    xn::EnumerationErrors errors;
    const char *fn = NULL;
    if (fileExists(SAMPLE_XML_PATH)) fn = SAMPLE_XML_PATH;
    else if (fileExists(SAMPLE_XML_PATH_LOCAL)) fn = SAMPLE_XML_PATH_LOCAL;
    else { printf("Could not find '%s' nor '%s'. Aborting.\n" , SAMPLE_XML_PATH, SAMPLE_XML_PATH_LOCAL); return XN_STATUS_ERROR; }
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


    //LIFE OF THE PARTY CODE BEGINS //HERE**************************************
    //**************************************************************************
    //**************************************************************************

    XnUserID aUsers[MAX_NUM_USERS];
    XnUInt16 nUsers;
    XnSkeletonJointTransformation head;
    XnSkeletonJointTransformation torso;
    XnSkeletonJointTransformation lfoot;
    XnSkeletonJointTransformation rfoot;
    XnSkeletonJointTransformation lhand;
    XnSkeletonJointTransformation rhand;


    char command[200];
    double fastestSpeed;
    jumpframegap = 0;
    printf("Starting to run\n");
    if(g_bNeedPose) printf("Assume calibration pose\n");

    while (!xnOSWasKeyboardHit())
    {
        g_Context.WaitOneUpdateAll(g_UserGenerator);
        nUsers=MAX_NUM_USERS;
        g_UserGenerator.GetUsers(aUsers, nUsers);

	fastestSpeed = 0;

        for(XnUInt16 userID=0; userID < nUsers; userID++)
        {

            if(g_UserGenerator.GetSkeletonCap().IsTracking(aUsers[userID])==FALSE) continue;
            g_UserGenerator.GetSkeletonCap().GetSkeletonJoint(aUsers[userID],XN_SKEL_RIGHT_HAND, rhand);

            // relative distance formula
            distancechange[userID] = sqrt(pow(rhand.position.position.X - lastx[userID], 2)
                    + pow(rhand.position.position.Y - lasty[userID],2) +
                    pow(rhand.position.position.Z - lastz[userID], 2));

            if((rhand.position.position.X - lastx[userID]) + (rhand.position.position.Y - lasty[userID]) > 0)
            {
                netpositivev[userID] = TRUE;
            }
            else
            {
                netpositivev[userID] = FALSE;
            }

            lastx[userID] = rhand.position.position.X;
            lasty[userID] = rhand.position.position.Y;
            lastz[userID] = rhand.position.position.Z;

            // calibrate user speed
            vaverage[userID][0] = ((distancechange[userID] + secondchange[userID]) * .001) / (.0667);
            secondchange[userID] = distancechange[userID];

            //determine the fastest user here

                if(vaverage[userID][0] > fastestSpeed){
                    fastestSpeed = vaverage[userID][0];
                    fastestUser = userID;
                }


            //COLOR JUMPING ALGORITHIM
            //If hand acceleration is greater than 2 m/s , complementary color
            //jump will occur

            // shift over all user's vel. average history
            for(int i = 5; i; i--){

                vaverage[userID][i] = vaverage[userID][i - 1];
            }

            printf("user %d velocity: %f\n", userID, vaverage[userID][0]);

        } // end user for loop

        //Number of color steps is determined by velocity
        steps = (int)(vaverage[fastestUser][0] * STEPCONST);

        //COLOR SHIFTING ALGORITHIM

        if(vaverage[fastestUser][0] > 1){

            if(netpositivev[fastestUser] ){
                printf("user %d net positive\n", fastestUser);

                for( int i = 0; i <= steps; i++){

                    if(r== 255.0 && b!= 255.0 && g== 0.0)  b++;
                    if(r== 255.0 && b== 255.0 && g== 0.0) r--;
                    if(r!= 255.0 && b== 255.0 && g== 0.0) r--;
                    if(r==  0.0  && b== 255.0 && g!= 255.0) g++;
                    if(r==  0.0  && b== 255.0 && g== 255.0) b--;
                    if(r==  0.0  && b!= 255.0 && g== 255.0) b--;
                    if(r!= 255.0 && b==  0.0  && g== 255.0) r++;
                    if(r== 255.0 && b==  0.0  && g== 255.0) g--;
                    if(r== 255.0 && b==  0.0  && g!= 0.0 ) g--;

                }

            }

            if(!netpositivev[fastestUser] ){
		printf("user %d net negative\n", fastestUser);
                for( int i = 0; i <= steps; i++){

                    if(r == 255.0 && b != 0.0 && g== 0.0) b--;
                    if(r== 255.0 && b== 0.0 && g!= 255.0) g++;
                    if(r!= 0.0 && b== 0.0 && g== 255.0) r--;
                    if(r== 0.0 && b!= 255.0 && g== 255.0) b++;
                    if(r== 0.0 && b== 255.0 && g!= 0.0) g--;
                    if(r!= 255.0 && b== 255.0 && g== 0.0) r++;
                }
            }

	    if(jumpframegap == 0){
         	   if(vaverage[fastestUser][0] - (vaverage[fastestUser][1] + vaverage[fastestUser][2]
					 + vaverage[fastestUser][3] + vaverage[fastestUser][4])/4 > 2.2){

         	      r = 255 - r;
	              g = 255 - g;
	              b = 255 - b;
                  jumpframegap = 10;
	              printf("JUMP*****************************************************\n");
		   }
            }else{jumpframegap--;}

        }

        sprintf(command, "python ../../../../LED-control/varcolor.py %f %f %f", r/255, g/255, b/255);
        system(command);

    }// end of kinect loop

    g_scriptNode.Release();
    g_UserGenerator.Release();
    g_Context.Release();

}
