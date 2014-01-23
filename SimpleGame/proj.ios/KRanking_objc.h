//
//  KRanking_objc.h
//  SimpleGame
//
//  Created by Guest on 2014/01/23.
//
//

//C++から呼ぶためにシングルトンにしている

#import <Foundation/Foundation.h>

@interface KRanking_objc : NSObject

// シングルトン
+ (KRanking_objc *)sharedInstance;

-(void)ranking_query_all;
-(void)ranking_post:(NSString*)name Score:(int)score;

@end
