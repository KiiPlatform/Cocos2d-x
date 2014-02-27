//
//  KRanking_objc.h
//  SimpleGame
//
//  Created by Guest on 2014/01/23.
//
//

//C++から呼ぶためにシングルトンにしている

#import <Foundation/Foundation.h>

@interface KRanking_objc : NSObject {
    NSString *_userDisplayName, *_username;
}

// シングルトン
+ (KRanking_objc *)sharedInstance;
-(void)regist;
-(void)loginWithToken;
-(void)ranking_query_all;
-(void)ranking_post:(NSString*)name Score:(int)score;
-(void)displayNameUpdate:(NSString*)name;

@end
