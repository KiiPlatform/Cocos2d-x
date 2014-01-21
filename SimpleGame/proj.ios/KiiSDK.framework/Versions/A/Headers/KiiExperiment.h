//
//  KiiExperiment.h
//  KiiSDK-Private
//
//  Created by Syah Riza on 9/18/13.
//  Copyright (c) 2013 Kii Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "KiiVariationSampler.h"

/**
* This enum represents the status of experiment.
*/
typedef NS_ENUM(NSInteger, KiiExperimentStatus) {
    /** The experiment is active. */
    Active,
    /** The experiment is paused. */
    Paused,
    /** The experiment is terminated. */
    Terminated
};

@class KiiExperiment;

typedef void(^KiiExperimentBlock)(KiiExperiment* experiment, NSError* error);

@class KiiVariation,KiiConversionEvent;
/** KiiExperiment is a class that handle experiment for AB testing feature.

 <KiiExperimentStatus> enumeration :

 - Active
 - Paused
 - Terminated
 */
@interface KiiExperiment : NSObject

/** Get experiment synchronously having the specified name.
  This is blocking methods.
 @param name experiment name.
 @param error An NSError object, passed by reference.
 @return an instant of KiiExperiment.
 @exception NSInvalidArgumentException Thrown if given experiment name is <br>
  - Nil or empty <br>
  - Does not match with the pattern [a-zA-Z0-9\-\_\.]\{2,100\}"<br>
 @exception KiiIllegalStateException will be thrown if there is no logged user.
 */
+ (KiiExperiment*) getExperimentSynchronous:(NSString*) name withError:(NSError**) error;

/** Get experiment asynchronously having the specified name with block.
 This is non-blocking methods.
 
     [KiiExperiment getExperiment:name withBlock:^(KiiExperiment *experiment, NSError *error) {
         if(error == nil) {
             NSLog(@"Experiment: %@", experiment);
         }
     }];
 
 @param name experiment name.
 @param completion The block to be called upon method completion. See example.
 @return an instant of KiiExperiment.
 @exception NSInvalidArgumentException Thrown if given experiment name is <br>
  - Nil or empty <br>
  - Does not match with the pattern [a-zA-Z0-9\-\_\.]\{2,100\}" <br>
 @exception KiiIllegalStateException will be thrown if there is no logged user.
 */
+ (void) getExperiment:(NSString*) name withBlock:(KiiExperimentBlock) completion;

/** Get the variation applied to this trial.
 Get the variation applied to this trial. Variation will be determined by specified rate of each variation in this experiment. 
 Current login user information will be used for sampling. If the experiment has finished with specified variant, the specified variant will be returned regardless of login user information. 
 If the experiment has been terminated without specified variant an error (code 902) will be returned.
 If the experiment has paused an error (code 903) will be returned.
 @param error An NSError object, passed by reference.
 @exception KiiIllegalStateException will be thrown if there is no logged user.
 */
- (KiiVariation*) appliedVariationWithError:(NSError**) error;

/** Get the variation applied to this trial. <br>
Sampler should return the variation according to the rate defined in this experiment.
If you use <KiiVariationSamplerByKiiUser> with current login user, it will be same as <appliedVariationWithError:> .
 @note An error will be returned if the experiment is not applied, depending on implementation of KiiVariationSampler.
 @param sampler an instance of object that conform KiiVariationSampler protocol.
 @param error An NSError object, passed by reference.
 @exception KiiIllegalStateException will be thrown if there is no logged user.
 */
- (KiiVariation*) appliedVariationWithSampler:(id<KiiVariationSampler>) sampler andError:(NSError**) error;

/** Get the variation which has the specified name.
 If variation with specified name is not found, an error (code 904) will be returned.
 
 @param name variation name.
 @param error An NSError object, passed by reference.
 @return an instant of KiiVariation. 
 */
- (KiiVariation*) variationByName:(NSString*) name andError:(NSError**) error;


/** Get conversion event by index.
 @param index an index of conversion. Should be on variations array bounds.
 @return an instance of KiiVariation, nil if there is no element found for specified index.
 */
- (KiiConversionEvent*) conversionEventAtIndex:(int) index;

/** Get conversion event by its name.
 @param name of the Conversion Event.
 @return an instance of KiiVariation, nil if there is no conversion found for specified name.
 */
- (KiiConversionEvent*) conversionEventByName:(NSString*) name;


/** Get the variations associated with this experiment.
 @return an array of KiiVariation, nil if there is no variation found.
 */
@property(nonatomic,readonly) NSArray* variations;

/** Get the version number of the experiment.
 */
@property(nonatomic,readonly) int version;

/** Get the status of the experiment.
 */
@property(nonatomic,readonly) KiiExperimentStatus status;

/** Get the name of the experiment.
 */
@property(nonatomic,readonly) NSString* name;

/** Get the description of the experiment.
 */
@property(nonatomic,readonly) NSString* description;

/** Get all conversion events in this experiment.
 */
@property(nonatomic,readonly) NSArray* conversionEvents;

/** Returns chosen variation if the experiment has finished with specified variation.
 @return Variation chosen or null if when the no variation has not been chosen.
 */
@property(nonatomic,readonly) KiiVariation* chosenVariation;

@end
