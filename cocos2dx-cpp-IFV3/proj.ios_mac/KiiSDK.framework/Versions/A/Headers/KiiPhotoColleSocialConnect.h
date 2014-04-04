//
//  KiiPhotoColleSocialConnect.h
//  KiiSDK-Private
//
//  Copyright (c) 2013 Kii Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "KiiUser.h"

@class KiiCloudPhotoColle;
@class UINavigationController;

/**
 The type of display
 */
typedef NS_ENUM(NSInteger, KiiDCDisplayType) {
    /** Diplay type for smart phone. */
    KIIDCDISPLAYTYPE_SMART_PHONE = 0,
    /** Diplay type for tablet. */
    KIIDCDISPLAYTYPE_TABLET = 1
};

/**
 * An interface to link users to PhotoColle network.
 *
 * KiiPhotoColleSocialConnect is singleton class. You can get the
 * singleton instance by <sharedInstance>. Before getting the
 * singleton instance, You need to set up KiiPhotoColleSocialConnect
 * by
 * <setupNetworkWithClientId:clientSecret:redirectUri:displayType:>.
 * If you create KiiPhotoColleSocialConnect with
 * `[[KiiPhotoColleSocialConnect alloc] init]`, the instance does not
 * work.<br><br>
 * Prior to use KiiPhotoColleSocialConnect, you must initialize
 * the Kii SDK by <[Kii beginWithID:andKey:andSite:]>
 * with kiiSiteJP as the value of Site argument.
 */
@interface KiiPhotoColleSocialConnect : NSObject

/**
 * Set up KiiPhotoColleSocialConnect.
 * You must call this method before calling other methods of
 * KiiPhotoColleSocialConnect.
 * @param clientId The client id string which is issued for your service.
 * @param clientSecret The client secret string which is issued for your
 * service.
 * @param redirectUri The redirect URI which you registered.
 * @param displayType Type of display of target device.  See
 * `DCDisplayType` for details. DCDisplayType is defined in
 * PhotoColleSDK.framework.
 * @exception NSInvalidArgumentException Thrown if:
 * <UL>
 *  <LI>ClientId, clientSecret or redirectUri is nil or empty.</LI>
 *  <LI>displayType is nil.</LI>
 * </UL>
 * @exception NSInternalInconsistencyException PhotoColleSDK.framework
 * is not linked.
 */
+ (void)setupNetworkWithClientId:(NSString *)clientId
                    clientSecret:(NSString *)clientSecret
                     redirectUri:(NSString *)redirectUri
                     displayType:(KiiDCDisplayType)displayType;

/**
 * Get KiiPhotoColleSocialConnect instance.
 * KiiPhotoColleSocialConnect must already be set up via <setupNetwork:withKey:andSecret:andOptions:>
 *
 * @exception KiiIllegalStateException If <setupNetworkWithClientId:clientSecret:redirectUri:displayType:>
 * has not been called.
 */
+ (KiiPhotoColleSocialConnect *)sharedInstance;

/**
 Log a user into PhotoColle network.

 Before calling this method, Application must already set up
 KiiPhotoColleSocialConnect by
 <setupNetworkWithClientId:clientSecret:redirectUri:displayType:>.

 This will initiate the login process for PhotoColle network. If
 Applications already logged into KiiCloud, then the logged in user
 are logged out. New user logs into KiiCloud and the new user is
 linked to PhotoColle network.

 Termination of authentication process is notified by
 completion block. Example of the block is following:

     // controller is a UINavigationController to show authentication
     // page. Application must implement and provide this
     // UINavigationController.
     
     [[KiiPhotoColleSocialConnect sharedInstance]
             logInOnNavigationController:controller
                            withBlock:^(KiiUser *user, NSError *error)
             {
                 if (error == nil) {
                     // Success. Do something which an application needs.
                 } else {
                     // There was a problem.
                 }
             }
      ];

 @param controller UINavigationController which is described in PhotoColle
 authentication page.
 @param completion The block to be called when the operation is completed.
 */
- (void)logInOnNavigationController:(UINavigationController *)controller
                          withBlock:(KiiUserBlock)completion;

/**
 Link the currently logged in user with PhotoColle network.

 Before calling this method, Application must already set up
 KiiPhotoColleSocialConnect by
 <setupNetworkWithClientId:clientSecret:redirectUri:displayType:>.

 This will initiate the login process for PhotoColle network with
 the currently logged in user. There must be a currently
 authenticated <KiiUser>. Otherwise, you can use the
 <logInOnNavigationController:withBlock:> to create and login a
 <KiiUser> using PhotoColle.

    // controller is a UINavigationController to show authentication
    // page. Application must implement and provide this
    // UINavigationController.
    
    [[KiiPhotoColleSocialConnect sharedInstance]
            linkCurrentUserOnNavigationController:controller
                                     withBlock:^(
                                         KiiUser *user,
                                         NSError *error)
            {
                if (error == nil) {
                    // Success. Do something which an application needs.
                } else {
                    // There was a problem.
                }
            }
     ];

 @param controller UINavigationController which is described PhotoColle
 authentication page.
 @param completion The block to be called when the operation is completed.
 */
- (void)linkCurrentUserOnNavigationController:(UINavigationController *)controller
                                    withBlock:(KiiUserBlock)completion;

/**
 * Unlink the currently logged in user from PhotoColle network.
 * @param completion The block method to be called when the operation is completed.
 */
- (void)unlinkCurrentUserWithBlock:(KiiUserBlock)completion;

/**
   Get <KiiCloudPhotoColle> object.

   Applications must link with PhotoColle network with
   <[KiiPhotoColleSocialConnect
   logInOnNavigationController:withBlock:]> or
   <[KiiPhotoColleSocialConnect
   linkCurrentUserOnNavigationController:withBlock:]> before
   getting <KiiCloudPhotoColle> object.

   <KiiCloudPhotoColle> instance holds credentials authenticated by
   <[KiiPhotoColleSocialConnect
   logInOnNavigationController:withBlock:]> or
   <[KiiPhotoColleSocialConnect
   linkCurrentUserOnNavigationController:withBlock:]>. The
   credentials will not change after <[KiiPhotoColleSocialConnect
   logInOnNavigationController:withBlock:]>,
   <[KiiPhotoColleSocialConnect
   linkCurrentUserOnNavigationController:withBlock:]>,
   <[KiiPhotoColleSocialConnect unlinkCurrentUserWithBlock:]> or
   <[KiiUser logOut]> called. Please make sure to invalidate the
   KiiCloudPhotoColle instance (ex. nullify) when you switch the login
   user.

   @return If one of following conditions is applied,
   <KiiCloudPhotoColle> instance is returned.

   * <[KiiPhotoColleSocialConnect logInOnNavigationController:withBlock:]> or <[KiiPhotoColleSocialConnect linkCurrentUserOnNavigationController:withBlock:]> has been executed and succeeded.
   * Current login user's photocolle token has been stored.

   Otherwise, returns null.
 */
- (KiiCloudPhotoColle *)kiiCloudPhotoColle;

@end
