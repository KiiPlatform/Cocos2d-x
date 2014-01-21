//
//  KiiPushMessage.h
//  KiiSDK-Private
//
//  Created by Riza Alaudin Syah on 1/24/13.
//  Copyright (c) 2013 Kii Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>

/** Enumeration of KiiMessageField that represents KiiCloud specific fields.

 KiiMessageField list is as follows:

  Key           | Short key | Push to App | Push to User                        | Direct Push                           | Description                                          | Possible values
 ---------------|-----------|-------------|-------------------------------------|---------------------------------------|------------------------------------------------------|------
 APP_ID         | a         | -           | -<br>(Depends on "sendAppID")       | -                                     | Source app which generated the notification.         |
 SENDER         | s         | -           | X<br>(Depends on "sendSender")      | X<br>(Depends on "sendSender")        | The user who caused the notification.                |
 ORIGIN         | o         | -           | -<br>(Depends on "sendOrigin")      | -<br>(Depends on "sendOrigin")        | Origin of push. "EVENT" for "Push to App" notification. "EXPLICIT" for "Push to User" and "Direct Push" notification. | - EVENT<br> - EXPLICIT
 WHEN           | w         | X           | -<br>(Depends on "sendWhen")        | -<br>(Depends on "sendWhen")          | The timestamp of the notification in milliseconds. (Since January 1, 1970 00:00:00 UTC) |
 TYPE           | t         | X           | -<br>(Depends on "pushMessageType") | -<br>(Depends on "pushMessageType")   | The type of notification and the additional data.    | [Push to App]<br>- DATA_OBJECT_CREATED<br> - DATA_OBJECT_DELETED<br> - DATA_OBJECT_UPDATED<br> - DATA_OBJECT_BODY_UPDATED<br> - DATA_OBJECT_BODY_DELETED<br> - DATA_OBJECT_ACL_MODIFIED<br> - BUCKET_DELETED
 TOPIC          | to        | -           | X<br>(Depends on "sendTopicID")     | -                                     | TopicID that is the source of this notification. TopicID is only for "Push to User" push messages.    |
 SCOPE_APP_ID   | sa        | X           | X<br>(Depends on "sendObjectScope") | -                                     | AppID of object scope.                               |
 SCOPE_USER_ID  | su        | X           | X<br>(Depends on "sendObjectScope") | -                                     | UserID of object scope. Push message has this field only if the subscribed bucket is user scope.|
 SCOPE_GROUP_ID | sg        | X           | X<br>(Depends on "sendObjectScope") | -                                     | GroupID of object scope. Push message has this field only if the subscribed bucket is group scope. |
 SCOPE_TYPE     | st        | X           | X<br>(Depends on "sendObjectScope") | -                                     | Type of object scope.                                 | - APP<br> - APP_AND_USER<br> - APP_AND_GROUP
 BUCKET_ID      | bi        | X           | -                                   | -                                     | Bucket name of push subscribed.                      |
 BUCKET_TYPE    | bt        | X           | -                                   | -                                     | Type of source bucket.                    | - rw<br> - sync
 OBJECT_ID      | oi        | X           | -                                   | -                                     | ID of the object operated.                           |
 OBJECT_MODIFIED_AT | om    | -           | -                                   | -                                     | Timestamp of the modification of object in milliseconds. (Since January 1, 1970 00:00:00 UTC)  | - DATA_OBJECT_CREATED<br> - DATA_OBJECT_UPDATED<br> - DATA_OBJECT_BODY_UPDATE

 */
typedef NS_ENUM(NSUInteger, KiiMessageField) {
    /** Source app which generated the notification. */
    APP_ID,
    /** The user who caused the notification. */
    SENDER,
    /** The type of notification and the additional data. */
    TYPE,
    /** The timestamp of the notification in milliseconds.
     (Since January 1, 1970 00:00:00 UTC) */
    WHEN,
    /** Origin of push. "EVENT" for "Push to App" notification.
     "EXPLICIT" for "Push to User" and "Direct Push" notification. */
    ORIGIN,
    /** TopicID that is the source of this notification.
     TopicID is only for "Push to User" push messages. */
    TOPIC,
    /** AppID of object scope. */
    SCOPE_APP_ID,
    /** UserID of object scope.
     Push message has this field only if the subscribed bucket is user scope. */
    SCOPE_USER_ID,
    /** GroupID of object scope.
     Push message has this field only if the subscribed bucket is group scope. */
    SCOPE_GROUP_ID,
    /** Type of object scope. */
    SCOPE_TYPE,
    /** Bucket name of push subscribed. */
    BUCKET_ID,
    /** Type of source bucket. */
    BUCKET_TYPE,
    /** ID of the object operated. */
    OBJECT_ID,
    /** Timestamp of the modification of object in milliseconds.
     (Since January 1, 1970 00:00:00 UTC) */
    OBJECT_MODIFIED_AT
};

@class KiiAPNSFields,KiiGCMFields;

/**
 Class for encapsulating incoming and outgoing push notification message
Three types of push message supported by KiiCloud.

- **Push to App** : Message sent to the subscribers when an event happens in the <KiiBucket> and <KiiFileBucket>.
- **Push to User** : Message sent to the subscribers of <KiiTopic> that is created explicitly.
- **Direct Push** : Message sent to a certain user by manipulating the developer portal. (Only app developer can send this message.)

Following section describes the contents of "Push to App", "Push to User" and "Direct Push" message.

### Contents of Push Message

Push message of "Push to App", "Push to User" and "Direct Push" contains the KiiCloud specific fields that are selected by the message sender.
And also push message of "Push to User" contains the basic message as key-value pair that is sent to the <KiiTopic>.

KiiCloud specific fields are as follows: 

  Key           | Short key | Push to App | Push to User                        | Direct Push                           | Description                                          | Possible values
 ---------------|-----------|-------------|-------------------------------------|---------------------------------------|------------------------------------------------------|------
 APP_ID         | a         | -           | -<br>(Depends on "sendAppID")       | -                                     | Source app which generated the notification.         |
 SENDER         | s         | -           | X<br>(Depends on "sendSender")      | X<br>(Depends on "sendSender")        | The user who caused the notification.                |
 ORIGIN         | o         | -           | -<br>(Depends on "sendOrigin")      | -<br>(Depends on "sendOrigin")        | Origin of push. "EVENT" for "Push to App" notification. "EXPLICIT" for "Push to User" notification. | - EVENT<br> - EXPLICIT
 WHEN           | w         | X           | -<br>(Depends on "sendWhen")        | -<br>(Depends on "sendWhen")          | The timestamp of the notification in milliseconds. (Since January 1, 1970 00:00:00 UTC) |
 TYPE           | t         | X           | -<br>(Depends on "pushMessageType") | -<br>(Depends on "pushMessageType")   | The type of notification and the additional data.    | [Push to App]<br>- DATA_OBJECT_CREATED<br> - DATA_OBJECT_DELETED<br> - DATA_OBJECT_UPDATED<br> - DATA_OBJECT_BODY_UPDATED<br> - DATA_OBJECT_BODY_DELETED<br> - DATA_OBJECT_ACL_MODIFIED<br> - BUCKET_DELETED
 TOPIC          | to        | -           | X<br>(Depends on "sendTopicID")     | -                                     | TopicID is only for "Push to User" push messages.    |
 SCOPE_APP_ID   | sa        | X           | X<br>(Depends on "sendObjectScope") | -                                     | AppID of object scope.                               |
 SCOPE_USER_ID  | su        | X           | X<br>(Depends on "sendObjectScope") | -                                     | UserID of object scope. Push message has this field only if the subscribed bucket is user scope.|
 SCOPE_GROUP_ID | sg        | X           | X<br>(Depends on "sendObjectScope") | -                                     | GroupID of object scope. Push message has this field only if the subscribed bucket is group scope. |
 SCOPE_TYPE     | st        | X           | X<br>(Depends on "sendObjectScope") | -                                     | Type of object scope.                                 | - APP<br> - APP_AND_USER<br> - APP_AND_GROUP 
 BUCKET_ID      | bi        | X           | -                                   | -                                     | Bucket name of push subscribed.                      |
 BUCKET_TYPE    | bt        | X           | -                                   | -                                     | Type of bucket has been modified.                    | - rw<br> - sync
 OBJECT_ID      | oi        | X           | -                                   | -                                     | ID of the object operated.                           |
 OBJECT_MODIFIED_AT | om    | -           | -                                   | -                                     | Timestamp of the modification of object in milliseconds. (Since January 1, 1970 00:00:00 UTC)  | - DATA_OBJECT_CREATED<br> - DATA_OBJECT_UPDATED<br> - DATA_OBJECT_BODY_UPDATE
 
 ### GCM restriction for reserved keyword
 Based on Google GCM specification, there are reserved payload keys that should not be used inside data/specific data.
 If GCM is enabled and the data contains one or more reserve keys, an error (code 712) will be thrown.
 Following are the list of GCM reserved keys:
 
    - any key prefix with 'google'
    - from
    - registration_ids
    - collapse_key
    - data
    - delay_while_idle
    - time_to_live
    - restricted_package_name
    - dry_run

 */
@interface KiiPushMessage : NSObject

/** Dictionary representation of APNs received message.
*/
@property(nonatomic,readonly) NSDictionary* rawMessage;

/** Dictionary representation of JSON Object with only one-level of nesting. Required if no system-specific “data” fields has been provided for all the systems enabled.	Dictionary with the data that will be sent to all the push systems enabled in this request.
 If gcmFields is defined, the data would be validated for GCM reserved payload keys. 
 */
@property(nonatomic,strong) NSDictionary* data;

/** APNS-specific fields.
 */
@property(nonatomic,strong) KiiAPNSFields* apnsFields;

/** GCM-specific fields.
 */
@property(nonatomic,strong) KiiGCMFields* gcmFields;

/**Boolean. Not required.
 If true this message will be sent to the devices that have the property "development" to "true" in their installations. Default is true.
 */
@property(nonatomic,strong) NSNumber* sendToDevelopment;

/** Boolean. Not required.
 If true this message will be sent to the devices that have the property "development" to "false" or null in their installations. Default is true.
 */
@property(nonatomic,strong) NSNumber* sendToProduction;

/** String. Not required.
 Value that will optionally indicate what is the type of the message. Event-generated push messages contain it.
 */
@property(nonatomic,strong) NSString* pushMessageType;

/** Boolean. Not required.
 If true, the appID field will also be sent. Default is false.
 */
@property(nonatomic,strong) NSNumber* sendAppID;

/**Boolean. Not required.
 If true, send the “sender” field (userID of the user that triggered the notification). Default is true.
 */
@property(nonatomic,strong) NSNumber* sendSender;

/** Boolean. Not required.
 If true, send the “when” field (when the push message was sent). Default is false.
 */
@property(nonatomic,strong) NSNumber* sendWhen;

/**Boolean. Not required.
 If true, send the “origin” field (indicates if the message is the result of an event or sent explicitly by someone. Default is false.
 */
@property(nonatomic,strong) NSNumber* sendOrigin;

/**Boolean. Not required.
 If true, send the “objectScope”-related fields that contain the topic that is the source of this notification. Default is true.
 */
@property(nonatomic,strong) NSNumber* sendObjectScope;

/** Boolean. Not required.
 If true, send the “topicID” field, which contains the topicID that is the source of this notification. Default is true.
 */
@property(nonatomic,strong) NSNumber* sendTopicID;

/** Parse incoming APNs message.
 @param userInfo An userInfo instance that received from APNs as a push message.
 @return A KiiPushMessage instance that is associated to the userInfo.
 */
+(KiiPushMessage*) messageFromAPNS:(NSDictionary*) userInfo;

/** Constructor method that composes a message for explicit push
 @param apnsfields The message data for APNS
 @param gcmfields The message data for GCM
 @return A KiiPushMessage instance that is associated to the message data.
 */
+(KiiPushMessage*) composeMessageWithAPNSFields:(KiiAPNSFields*) apnsfields andGCMFields:(KiiGCMFields*)gcmfields;

/** Get specific value of received message meta data.
@param field Enumeration of <KiiMessageField>.
@return A NSString object that is associated to the message fields.
*/
-(NSString*) getValueOfKiiMessageField:(KiiMessageField) field;

/** Get alert body's text message.
 @return A NSString object of alert body text message.
 */
-(NSString*) getAlertBody;

/** Show simple alert to display alert body's message.
 @param title A alert title to display.
 */
-(void) showMessageAlertWithTitle:(NSString*) title;


@end
