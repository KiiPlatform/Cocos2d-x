/****************************************************************************
 Copyright (c) 2010 cocos2d-x.org
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
#import <UIKit/UIKit.h>
#import "AppController.h"
#import "cocos2d.h"
#import "EAGLView.h"
#import "AppDelegate.h"

#import <KiiSDK/Kii.h>

#import "RootViewController.h"

//class HelloWorld;
//extern G_label_buff[];

void ranking_query_all2(){
    NSLog(@"ranking_query_all2");
    
    AppController *appController = (AppController *)[UIApplication sharedApplication].delegate;
    [appController ranking_query_all];
    
}

@implementation AppController

#pragma mark -
#pragma mark Application lifecycle

// cocos2d application instance
static AppDelegate s_sharedApplication;

static  NSString *APPID = @"ee573743";
static  NSString *APPKEY = @"5eb7b8bc1b4e4c98e659431c69cef8d4";


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {    
    
    NSLog(@"didFinishLaunchingWithOptions");
    
    // Override point for customization after application launch.

    // Add the view controller's view to the window and display.
    window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];
    EAGLView *__glView = [EAGLView viewWithFrame: [window bounds]
                                        pixelFormat: kEAGLColorFormatRGBA8
                                        depthFormat: GL_DEPTH_COMPONENT16
                                 preserveBackbuffer: NO
                                                                                 sharegroup:nil
                                                                          multiSampling:NO
                                                                    numberOfSamples:0];
    
    // Use RootViewController manage EAGLView 
    viewController = [[RootViewController alloc] initWithNibName:nil bundle:nil];
    viewController.wantsFullScreenLayout = YES;
    viewController.view = __glView;

    // Set RootViewController to window
    if ( [[UIDevice currentDevice].systemVersion floatValue] < 6.0)
    {
        // warning: addSubView doesn't work on iOS6
        [window addSubview: viewController.view];
    }
    else
    {
        // use this method on ios6
        [window setRootViewController:viewController];
    }
    
    [window makeKeyAndVisible];

    [[UIApplication sharedApplication] setStatusBarHidden: YES];
    
    cocos2d::CCApplication::sharedApplication()->run();
    
    //kii
    //Statup Kii platform ... just one line to enable Kii Cloud integaration
    [Kii beginWithID:APPID andKey:APPKEY andSite:kiiSiteUS];
    
    // Login
    NSString* username = @"muku";
    NSString* password = @"1234";
    [KiiUser authenticate:username withPassword:password andDelegate:self andCallback:@selector(authProcessComplete:withError:)];
    
    return YES;
}

// kii
- (void) authProcessComplete:(KiiUser*)user withError:(KiiError*)error {
    NSLog(@"authProcessComplete");
    
    if(error == nil) {
        NSLog(@"ok");
        
        //[self ranking_query_all];
        
    } else {
        NSLog(@"error %@",error);
    }
}

- (void)ranking_query_all {
    NSLog(@"ranking_query_all");
    
    //m_appRankingBucket
    KiiBucket *bucket1 = [Kii bucketWithName:@"b_ranking"];
    
    NSError *error = nil;
    KiiQuery *all_query = [KiiQuery queryWithClause:nil];
    [all_query sortByDesc:@"score"];
    NSMutableArray *allResults = [NSMutableArray array];
    KiiQuery *nextQuery;
    NSArray *results = [bucket1 executeQuerySynchronous:all_query
                                              withError:&error
                                                andNext:&nextQuery];
    [allResults addObjectsFromArray:results];
    //NSLog(@"allResults %@ ", allResults);
    
    //ログ表示
    int size = [allResults count];
    NSMutableArray *mArray = [NSMutableArray array];
    char buff[1000];
    for(int i=0;i<size; i++){
        KiiObject* obj=[allResults objectAtIndex:i];
        //NSLog(@"obj %@",obj);
        
        NSString* name=[[obj dictionaryValue] objectForKey:@"name"];
        NSString* score=[[obj dictionaryValue] objectForKey:@"score"];
        NSLog(@"%@ %@",name, score);
        
        [mArray addObject:@{@"name" : name,@"score" : score}];
        
        //c++へ
        //const char* c_name = [name UTF8String];
        //const char* c_score = [score UTF8String];
        //sprintf(buff,"%s : %s \n", c_name, c_score );
        //strcat(G_label_buff, buff);//í«â¡
    }

    //id dict = [NSDictionary dictionaryWithObjectsAndKeys:@"1",@"hoge",@"2",@"fuga", nil];
    //id dict = [NSArray allResults];
    
    /***
    NSMutableArray *mArray = [NSMutableArray array];
    [mArray addObject:@{@"name" : @"名前1",@"score" : @"12341"}];
    [mArray addObject:@{@"name" : @"名前2",@"score" : @"12342"}];
    [mArray addObject:@{@"name" : @"名前3",@"score" : @"12343"}];
     ***/

    
    NSError *error2 = nil;
    NSData *data = nil;
    NSString* json_str = nil;
    if([NSJSONSerialization isValidJSONObject:mArray]){
        NSLog(@"true isValidJSONObject");
        data = [NSJSONSerialization dataWithJSONObject:mArray options:NSJSONReadingAllowFragments error:&error2];
        //NSLog(@"%@",data);
        //NSLog(@"%@",[[[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding]autorelease]);
        json_str = [[[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding]autorelease];
        NSLog(@"json_str %@", json_str);
    } else {
        NSLog(@"false isValidJSONObject");
    }

    
}


- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
    cocos2d::CCDirector::sharedDirector()->pause();
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
    cocos2d::CCDirector::sharedDirector()->resume();
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
    cocos2d::CCApplication::sharedApplication()->applicationDidEnterBackground();
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
    cocos2d::CCApplication::sharedApplication()->applicationWillEnterForeground();
}

- (void)applicationWillTerminate:(UIApplication *)application {
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
}


#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
}


- (void)dealloc {
    [super dealloc];
}


@end

