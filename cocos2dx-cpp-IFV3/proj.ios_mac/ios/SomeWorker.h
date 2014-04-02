//
//  SomeWorker.h
//  cocos2dx-cpp-IFV3
//
//  Created by Guest on 2014/03/20.
//
//

#import <Foundation/Foundation.h>

@interface SomeWorker : NSObject

// idとselectorを引数で受け取るメソッドを定義します。
-(void)doSomeWorkWith:(id)callbackInstance selector:(SEL)callbackSelector;

@end
