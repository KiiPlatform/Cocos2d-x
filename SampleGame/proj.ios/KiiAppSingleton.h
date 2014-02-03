//
//  KiiAppSingleton.h
//  KiiPhotos
//
//  Created by Riza Alaudin Syah on 10/30/12.
//  Copyright (c) 2012 Kii Corporation. All rights reserved.
//  
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.

#import <Foundation/Foundation.h>


@class KiiUser;
/* This is singleton class.
 
 */
@class Reachability,WBErrorNoticeView;
@interface KiiAppSingleton : NSObject{
    KiiUser* currentUser;
    
}

@property(nonatomic,strong)     WBErrorNoticeView *errorNotice;
@property(nonatomic,strong)     Reachability* internetReach;
@property(nonatomic,strong)     KiiUser* currentUser;
@property(nonatomic,strong)     NSString* selectedCountry;
@property(nonatomic, strong)    NSString* selectedObjectURI;
@property(nonatomic,assign)     BOOL needToRefresh;
+(KiiAppSingleton*) sharedInstance;

-(void) registerToken;
-(BOOL) checkUserToken;
-(BOOL) loginWithToken;
-(void)doLogOut;

-(void) setUUID:(NSString*) uuid;
-(NSString*) getUUID;

-(void) setUserName:(NSString*) uuid;
-(NSString*) getUserName;

-(void) setPassword:(NSString*) uuid;
-(NSString*) getPassword;

@end
