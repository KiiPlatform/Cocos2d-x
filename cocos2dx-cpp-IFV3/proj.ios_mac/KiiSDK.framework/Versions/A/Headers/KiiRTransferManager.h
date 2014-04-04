//
//  KiiRTransferManager.h
//  KiiSDK-Private
//
//  Created by Syah Riza on 3/26/13.
//  Copyright (c) 2013 Kii Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "KiiRTransferManaging.h"

typedef void(^KiiRTransferManagerBlock)(id<KiiRTransferManaging> *transferManager,NSArray *uploadEntries, NSError *error);

@class KiiUser;
@protocol KiiRTransfer;

/**
 Manages status of resumable transfer.
*/
@interface KiiRTransferManager : NSObject <KiiRTransferManaging>

/**
 Upload entries are stored with identifier of KiiUser who execute the upload.
 This API get existing upload entries initiated by specified user.
 If specified user is null, it will list upload entries initiated by anonymous user.
 By default anonymous user can not upload object body unless configure object ACL explicitly. Refer to <KiiACL> about the details of ACL.
 This is blocking method.<br>
 
 <b>Entry Life cycle:</b> The entry will be created
 on calling <[KiiRTransfer transferWithProgressBlock:andError:]> and deleted
 on completion/termination of upload.
 
 @param error An NSError object, set to nil, to test for errors
 @return NSArray Upload entries array.
 */
-(NSArray*) getUploadEntriesByInitiator:(KiiUser*) user withError:(NSError**) error;

/**
 Download entries are stored with identifier of KiiUser who execute the download.
 This API get existing download entries initiated by specified user.
 If specified user is null, it will list download entries initiated by anonymous user.
 This is blocking method. <br>
 
 <b>Entry Life cycle:</b> The entry will be created
 on calling <[KiiRTransfer transferWithProgressBlock:andError:]> and deleted
 on completion/termination of download.
 
 @param error An NSError object, set to nil, to test for errors
 @return NSArray download entries array that status is ONGOING and SUSPENDED.
 */
-(NSArray*) getDownloadEntriesByInitiator:(KiiUser*) user withError:(NSError**) error;

@end
