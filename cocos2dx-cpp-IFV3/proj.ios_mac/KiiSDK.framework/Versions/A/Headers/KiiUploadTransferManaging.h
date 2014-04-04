//
//  KiiUploadTransferManaging.h
//  KiiSDK-Private
//
//  Copyright (c) 2014 Kii Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
 * A protocol to manage status of resumable upload.
 */
@protocol KiiUploadTransferManaging <NSObject>

/**
 Upload entries are stored with identifier of KiiUser who execute the upload.
 This API get existing upload entries initiated by current logged in user.
 If no user logged in, lists upload entries initiated by anonymous user.
 By default anonymous user can not upload object body unless configure object ACL explicitly. Refer to <KiiACL> about the details of ACL.
 This is blocking method.<br>

 <b>Entry Life cycle:</b> The entry will be created
 on calling <[KiiRTransfer transferWithProgressBlock:andError:]> and deleted
 on completion/termination of upload.
 
 @param error An NSError object, set to nil, to test for errors
 @return NSArray Upload entries array.
 */
-(NSArray*) getUploadEntries:(NSError**) error;

@end
