#import <UIKit/UIKit.h>

#import "HUBComponent.h"
#import "HUBJSONCompatibleBuilder.h"
#import "HUBSerializable.h"

NS_ASSUME_NONNULL_BEGIN

/**
 *  Load the view for a component if it hasn't been loaded already
 *
 *  @param component The component to load a view for
 *
 *  This function asserts that a view has been loaded after -loadView was sent to the component.
 */
static inline UIView *HUBComponentLoadViewIfNeeded(id<HUBComponent> component) {
    if (component.view == nil) {
        [component loadView];
    }
    
    UIView * const view = component.view;
    NSCAssert(view, @"All components are required to load a view in -loadView");
    return view;
}

/**
 *  Add binary JSON data to a JSON compatible builder
 *
 *  @param data The binary data to add to the builder. Must contain dictionary-based JSON.
 *  @param builder The builder to add the data to
 *
 *  @return Any error that was encountered when parsing the supplied JSON data, or nil if the operation
 *          was successfully completed.
 */
static inline NSError * _Nullable HUBAddJSONDataToBuilder(NSData *data, id<HUBJSONCompatibleBuilder> builder) {
    NSError *error;
    NSObject *JSONObject = [NSJSONSerialization JSONObjectWithData:data options:(NSJSONReadingOptions)0 error:&error];
    
    if (error != nil || JSONObject == nil) {
        return error;
    }
    
    if (![JSONObject isKindOfClass:[NSDictionary class]]) {
        return [NSError errorWithDomain:@"spotify.com.hubFramework.invalidJSON" code:0 userInfo:nil];
    }
    
    [builder addDataFromJSONDictionary:(NSDictionary *)JSONObject];
    
    return nil;
}

/**
 *  Merge two dictionaries and return the result
 *
 *  @param dictionaryA The first dictionary
 *  @param dictionaryB The second dictionary
 *
 *  If either of the dictionaries is `nil`, then the other dictionary is returned unmodified. Otherwise, the entries from `dictionaryB`
 *  will be added to `dictionaryA`, overriding any values that have duplicate keys in both dictionaries.
 */
static inline NSDictionary<NSString *, id> * _Nullable HUBMergeDictionaries(NSDictionary<NSString *, id> * _Nullable dictionaryA,
                                                                            NSDictionary<NSString *, id> * _Nullable dictionaryB) {
    if (dictionaryA == nil) {
        return dictionaryB;
    }
    
    if (dictionaryB == nil) {
        return dictionaryA;
    }
    
    NSMutableDictionary<NSString *, id> * const mergedDictionary = [dictionaryA mutableCopy];
    [mergedDictionary addEntriesFromDictionary:(NSDictionary *)dictionaryB];
    return [mergedDictionary copy];
}

/**
 *  Return a serialized string representation of a serializable object
 *
 *  @param object The object to return a serialized string for
 *
 *  @return A string containing a serialized representation of the object (JSON), or nil if the operation failed
 */
static inline NSString * _Nullable HUBSerializeToString(id<HUBSerializable> object) {
    NSDictionary * const serialization = [object serialize];
    NSData * const jsonData = [NSJSONSerialization dataWithJSONObject:serialization options:NSJSONWritingPrettyPrinted error:nil];
    
    if (jsonData == nil) {
        return nil;
    }
    
    return [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
}

NS_ASSUME_NONNULL_END
