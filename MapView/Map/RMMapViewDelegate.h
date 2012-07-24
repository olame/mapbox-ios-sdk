//
//  RMMapViewDelegate.h
//
// Copyright (c) 2008-2012, Route-Me Contributors
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

@class RMMapView;
@class RMMapLayer;
@class RMMarker;
@class RMAnnotation;
@class RMUserLocation;

typedef enum {
    RMUserTrackingModeNone              = 0,
    RMUserTrackingModeFollow            = 1,
    RMUserTrackingModeFollowWithHeading = 2
} RMUserTrackingMode;

/** The RMMapViewDelegate protocol defines a set of optional methods that you can use to receive map-related update messages. Because many map operations require the RMMapView class to load data asynchronously, the map view calls these methods to notify your application when specific operations complete. The map view also uses these methods to request annotation layers and to manage interactions with those layers. */
@protocol RMMapViewDelegate <NSObject>
@optional

/** @name Managing Annotation Layers */

/** Returns the layer associated with the specified annotation object. 
 
 An annotation layer can be created using RMMapLayer and its subclasses, such as RMMarker for points and RMShape for shapes such as lines and polygons.
 
 If the object in the annotation parameter is an instance of the RMUserLocation class, you can provide a custom view to denote the user’s location. To display the user’s location using the default system view, return `nil`. **FIXME**
 
 If you do not implement this method, or if you return `nil` from your implementation for annotations other than the user location annotation, the map view does not display a layer for the annotation.
 
 @param mapView The map view that requested the annotation layer.
 @param annotation The object representing the annotation that is about to be displayed. In addition to your custom annotations, this object could be an RMUserLocation object representing the user’s current location.

 @return The annotation layer to display for the specified annotation or `nil` if you do not want to display a layer.
*/
- (RMMapLayer *)mapView:(RMMapView *)mapView layerForAnnotation:(RMAnnotation *)annotation;

/** Tells the delegate that the visible layer for an annotation is about to be hidden from view due to scrolling or zooming the map. 
 
 @param mapView The map view whose annotation alyer will be hidden.
 @param annotation The annotation whose layer will be hidden.
*/
- (void)mapView:(RMMapView *)mapView willHideLayerForAnnotation:(RMAnnotation *)annotation;

/** Tells the delegate that the visible layer for an annotation has been hidden from view due to scrolling or zooming the map.
 
 @param mapView The map view whose annotation layer was hidden.
 @param annotation The annotation whose layer was hidden.
*/
- (void)mapView:(RMMapView *)mapView didHideLayerForAnnotation:(RMAnnotation *)annotation;

/** @name Responding to Map Position Changes */

- (void)beforeMapMove:(RMMapView *)map;
- (void)afterMapMove:(RMMapView *)map;

- (void)beforeMapZoom:(RMMapView *)map;
- (void)afterMapZoom:(RMMapView *)map;

/** Tells the delegate that the region displayed by the map view just changed.

 This method is called whenever the currently displayed map region changes. During scrolling, this method may be called many times to report updates to the map position. Therefore, your implementation of this method should be as lightweight as possible to avoid affecting scrolling performance.
 
 @param mapView The map view whose visible region changed. 
*/
- (void)mapViewRegionDidChange:(RMMapView *)mapView;

/** @name Responding to User Map Gestures */

- (void)doubleTapOnMap:(RMMapView *)map at:(CGPoint)point;
- (void)singleTapOnMap:(RMMapView *)map at:(CGPoint)point;
- (void)singleTapTwoFingersOnMap:(RMMapView *)map at:(CGPoint)point;
- (void)longSingleTapOnMap:(RMMapView *)map at:(CGPoint)point;

/** @name Responding to User Annotation Gestures */

- (void)tapOnAnnotation:(RMAnnotation *)annotation onMap:(RMMapView *)map;
- (void)doubleTapOnAnnotation:(RMAnnotation *)annotation onMap:(RMMapView *)map;
- (void)tapOnLabelForAnnotation:(RMAnnotation *)annotation onMap:(RMMapView *)map;
- (void)doubleTapOnLabelForAnnotation:(RMAnnotation *)annotation onMap:(RMMapView *)map;

- (BOOL)mapView:(RMMapView *)map shouldDragAnnotation:(RMAnnotation *)annotation;
- (void)mapView:(RMMapView *)map didDragAnnotation:(RMAnnotation *)annotation withDelta:(CGPoint)delta;
- (void)mapView:(RMMapView *)map didEndDragAnnotation:(RMAnnotation *)annotation;

/** @name Tracking the User Location */

/** Tells the delegate that the map view will start tracking the user’s position.
 
 This method is called when the value of the showsUserLocation property changes to YES.
 
 @param mapView The map view that is tracking the user’s location.
*/
- (void)mapViewWillStartLocatingUser:(RMMapView *)mapView;

/** Tells the delegate that the map view stopped tracking the user’s location.
 
 This method is called when the value of the showsUserLocation property changes to NO.
 
 @param mapView The map view that stopped tracking the user’s location.
*/
- (void)mapViewDidStopLocatingUser:(RMMapView *)mapView;

/** Tells the delegate that the location of the user was updated.
 
 While the showsUserLocation property is set to YES, this method is called whenever a new location update is received by the map view. This method is also called if the map view’s user tracking mode is set to RMUserTrackingModeFollowWithHeading and the heading changes.
 
 This method is not called if the application is currently running in the background. If you want to receive location updates while running in the background, you must use the Core Location framework.
 
 @param mapView The map view that is tracking the user’s location.
 @param userLocation The location object representing the user’s latest location.
*/
- (void)mapView:(RMMapView *)mapView didUpdateUserLocation:(RMUserLocation *)userLocation;

/** Tells the delegate that an attempt to locate the user’s position failed.
 
 @param mapView The map view that is tracking the user’s location.
 @param error An error object containing the reason why location tracking failed.
*/
- (void)mapView:(RMMapView *)mapView didFailToLocateUserWithError:(NSError *)error;

/** Tells the delegate that the user tracking mode changed. 
 
 @param mapView The map view whose user tracking mode changed.
 @param mode The mode used to track the user’s location.
 @param animated If YES, the change from the current mode to the new mode is animated; otherwise, it is not. This parameter affects only tracking mode changes. Changes to the user location or heading are always animated.
*/
- (void)mapView:(RMMapView *)mapView didChangeUserTrackingMode:(RMUserTrackingMode)mode animated:(BOOL)animated;

@end
