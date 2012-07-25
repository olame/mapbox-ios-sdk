//
//  RMMapView.h
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

#import <UIKit/UIKit.h>
#import <CoreGraphics/CGGeometry.h>

#import "RMGlobalConstants.h"
#import "RMFoundation.h"
#import "RMMapViewDelegate.h"
#import "RMTile.h"
#import "RMProjection.h"
#import "RMMapOverlayView.h"
#import "RMMapTiledLayerView.h"
#import "RMMapScrollView.h"
#import "RMTileSourcesContainer.h"

#define kRMUserLocationAnnotationTypeName   @"RMUserLocationAnnotation"
#define kRMTrackingHaloAnnotationTypeName   @"RMTrackingHaloAnnotation"
#define kRMAccuracyCircleAnnotationTypeName @"RMAccuracyCircleAnnotation"

@class RMProjection;
@class RMFractalTileProjection;
@class RMTileCache;
@class RMMapLayer;
@class RMMapTiledLayerView;
@class RMMapScrollView;
@class RMMarker;
@class RMAnnotation;
@class RMQuadTree;
@class RMUserLocation;


// constants for boundingMask
enum : NSUInteger {
    RMMapNoMinBound		= 0, // Map can be zoomed out past view limits
    RMMapMinHeightBound	= 1, // Minimum map height when zooming out restricted to view height
    RMMapMinWidthBound	= 2  // Minimum map width when zooming out restricted to view width (default)
};

/// The mode used to control map scrolling deceleration.
typedef enum : NSUInteger {
    RMMapDecelerationNormal = 0, /// The default deceleration rate for a map view.
    RMMapDecelerationFast   = 1, /// A fast deceleration rate for a map view.
    RMMapDecelerationOff    = 2  /// No deceleration for a map view. Scrolls stop immediately. 
} RMMapDecelerationMode;

/** An RMMapView object provides an embeddable map interface, similar to the one provided by Apple's MapKit. You use this class as-is to display map information and to manipulate the map contents from your application. You can center the map on a given coordinate, specify the size of the area you want to display, and annotate the map with custom information.
    
 @warning **Important:** Please note that you are responsible for getting permission to use the map data, and for ensuring your use adheres to the relevant terms of use.
*/
@interface RMMapView : UIView <UIScrollViewDelegate, UIGestureRecognizerDelegate, RMMapScrollViewDelegate, CLLocationManagerDelegate>

/** @name Accessing the Delegate */

/** The receiver's delegate. 

 A map view sends messages to its delegate regarding the loading of map data and changes in the portion of the map being displayed. The delegate also manages the annotation views used to highlight points of interest on the map.

 The delegate should implement the methods of the RMMapViewDelegate protocol.
*/
@property (nonatomic, assign) id <RMMapViewDelegate> delegate;

/** @name Accessing Map Properties */

#pragma mark - View properties

/** A Boolean value that determines whether the user may scroll around the map.
 
 This property controls only user interactions with the map. If you set the value of this property to NO, you may still change the map location programmatically.
 
 The default value of this property is YES.
*/
@property (nonatomic, assign) BOOL enableDragging;

/** A Boolean value that determines whether the map view bounces past the edge of content and back again and whether it animates the content scaling when the scaling exceeds the maximum or minimum limits. 
 
If the value of this property is YES, the map view bounces when it encounters a boundary of the content or when zooming exceeds either the maximum or minimum limits for scaling. Bouncing visually indicates that scrolling or zooming has reached an edge of the content. If the value is NO, scrolling and zooming stop immediately at the content boundary without bouncing. 
 
 The default value is NO.
*/
@property (nonatomic, assign) BOOL enableBouncing;

/** A Boolean value that determines whether double-tap zooms of the map always zoom on the center of the map, or whether they zoom on the center of the double-tap gesture. The default value is NO.
*/
@property (nonatomic, assign) BOOL zoomingInPivotsAroundCenter;

/** The mode used when decelerating the map scrolling. 

 Possible values are described in "RMMapDecelerationMode."
*/
@property (nonatomic, assign) RMMapDecelerationMode decelerationMode;

@property (nonatomic, assign)   double metersPerPixel;
@property (nonatomic, readonly) double scaledMetersPerPixel;
@property (nonatomic, readonly) double scaleDenominator; // The denominator in a cartographic scale like 1/24000, 1/50000, 1/2000000.
@property (nonatomic, readonly) float screenScale;

/** A Boolean value that adjusts the display of map tile images for retina-capable screens. 
 
 If set to YES, the map tiles are drawn at double size, typically 512 pixels square instead of 256 pixels, in order to compensate for smaller features and to make them more legible. If tiles designed for retina devices are used, this value should be set to NO in order to display these tiles at the proper size. The deafults value is NO.
*/
@property (nonatomic, assign)   BOOL adjustTilesForRetinaDisplay;
@property (nonatomic, readonly) float adjustedZoomForRetinaDisplay; // takes adjustTilesForRetinaDisplay and screen scale into account

/** @name Tracking the User Location */

/** A Boolean value indicating whether the map may display the user location.
 
 This property does not indicate whether the user’s position is actually visible on the map, only whether the map view is allowed to display it. To determine whether the user’s position is visible, use the userLocationVisible property. The default value of this property is NO.
 
 Setting this property to YES causes the map view to use the Core Location framework to find the current location. As long as this property is YES, the map view continues to track the user’s location and update it periodically.
*/
@property (nonatomic) BOOL showsUserLocation;

/** The annotation object representing the user’s current location. (read-only) */
@property (nonatomic, readonly, retain) RMUserLocation *userLocation;

/** A Boolean value indicating whether the device’s current location is visible in the map view. (read-only)
 
 This property uses the horizontal accuracy of the current location to determine whether the user’s location is visible. Thus, this property is YES if the specific coordinate is offscreen but the rectangle surrounding that coordinate (and defined by the horizontal accuracy value) is partially onscreen.
 
 If the user’s location cannot be determined, this property contains the value NO.
*/
@property (nonatomic, readonly, getter=isUserLocationVisible) BOOL userLocationVisible;

/** The mode used to track the user location.
 
 Possible values are described in “RMUserTrackingMode.”
*/
@property (nonatomic) RMUserTrackingMode userTrackingMode;

/** @name Attributing Map Data */

/** An instance of UIViewController that is the view controller presenting the map view. This view controller will receive a page curl transition to display the attribution information when the user taps the appropriate user interface element. 
*/
@property (weak) UIViewController *viewControllerPresentingAttribution;

// take missing tiles from lower zoom levels, up to #missingTilesDepth zoom levels (defaults to 0, which disables this feature)
@property (nonatomic, assign) NSUInteger missingTilesDepth;

@property (nonatomic, assign) NSUInteger boundingMask;

// subview for the background image displayed while tiles are loading.
@property (nonatomic, retain) UIView *backgroundView;

@property (nonatomic, assign) BOOL debugTiles;

#pragma mark - Initializers

- (id)initWithFrame:(CGRect)frame andTilesource:(id <RMTileSource>)newTilesource;

// designated initializer
- (id)initWithFrame:(CGRect)frame
      andTilesource:(id <RMTileSource>)newTilesource
   centerCoordinate:(CLLocationCoordinate2D)initialCenterCoordinate
          zoomLevel:(float)initialZoomLevel
       maxZoomLevel:(float)maxZoomLevel
       minZoomLevel:(float)minZoomLevel
    backgroundImage:(UIImage *)backgroundImage;

- (void)setFrame:(CGRect)frame;

#pragma mark - Movement

@property (nonatomic, assign) CLLocationCoordinate2D centerCoordinate;
@property (nonatomic, assign) RMProjectedPoint centerProjectedPoint;

// recenter the map on #coordinate, expressed as CLLocationCoordinate2D (latitude/longitude)
- (void)setCenterCoordinate:(CLLocationCoordinate2D)coordinate animated:(BOOL)animated;

// recenter the map on #aPoint, expressed in projected meters
- (void)setCenterProjectedPoint:(RMProjectedPoint)aPoint animated:(BOOL)animated;

- (void)moveBy:(CGSize)delta;

/** @name Manipulating the Visible Portion of the Map */

#pragma mark - Zoom

// minimum and maximum zoom number allowed for the view. #minZoom and #maxZoom must be within the limits of #tileSource but can be stricter; they are clamped to tilesource limits (minZoom, maxZoom) if needed.
@property (nonatomic, assign) float zoom;
@property (nonatomic, assign) float minZoom;
@property (nonatomic, assign) float maxZoom;

@property (nonatomic, assign) RMProjectedRect projectedBounds;
@property (nonatomic, readonly) RMProjectedPoint projectedOrigin;
@property (nonatomic, readonly) RMProjectedSize projectedViewSize;

// recenter the map on #boundsRect, expressed in projected meters
- (void)setProjectedBounds:(RMProjectedRect)boundsRect animated:(BOOL)animated;

- (void)zoomByFactor:(float)zoomFactor near:(CGPoint)center animated:(BOOL)animated;

- (void)zoomInToNextNativeZoomAt:(CGPoint)pivot animated:(BOOL)animated;
- (void)zoomOutToNextNativeZoomAt:(CGPoint)pivot animated:(BOOL)animated;

- (void)zoomWithLatitudeLongitudeBoundsSouthWest:(CLLocationCoordinate2D)southWest northEast:(CLLocationCoordinate2D)northEast animated:(BOOL)animated;

- (float)nextNativeZoomFactor;
- (float)previousNativeZoomFactor;

- (void)setMetersPerPixel:(double)newMetersPerPixel animated:(BOOL)animated;

#pragma mark - Bounds

// returns the smallest bounding box containing the entire view
- (RMSphericalTrapezium)latitudeLongitudeBoundingBox;
// returns the smallest bounding box containing a rectangular region of the view
- (RMSphericalTrapezium)latitudeLongitudeBoundingBoxFor:(CGRect) rect;

- (BOOL)tileSourceBoundsContainProjectedPoint:(RMProjectedPoint)point;

- (void)setConstraintsSouthWest:(CLLocationCoordinate2D)southWest northEast:(CLLocationCoordinate2D)northEast;
- (void)setProjectedConstraintsSouthWest:(RMProjectedPoint)southWest northEast:(RMProjectedPoint)northEast;

/** @name Accessing Snapshot Images of the Map */

#pragma mark - Snapshots

- (UIImage *)takeSnapshot;
- (UIImage *)takeSnapshotAndIncludeOverlay:(BOOL)includeOverlay;

/** @name Annotating the Map */

#pragma mark - Annotations

@property (nonatomic, readonly) NSArray *annotations;
@property (nonatomic, readonly) NSArray *visibleAnnotations;

- (void)addAnnotation:(RMAnnotation *)annotation;
- (void)addAnnotations:(NSArray *)annotations;

- (void)removeAnnotation:(RMAnnotation *)annotation;
- (void)removeAnnotations:(NSArray *)annotations;
- (void)removeAllAnnotations;

- (CGPoint)mapPositionForAnnotation:(RMAnnotation *)annotation;

#pragma mark - TileSources

@property (nonatomic, retain) RMQuadTree *quadTree;

@property (nonatomic, assign) BOOL enableClustering;
@property (nonatomic, assign) BOOL positionClusterMarkersAtTheGravityCenter;
@property (nonatomic, assign) CGSize clusterMarkerSize;
@property (nonatomic, assign) CGSize clusterAreaSize;

@property (nonatomic, retain)   RMTileCache *tileCache;

/** @name Managing Map Tile Sources and Layering */

@property (nonatomic, readonly) RMTileSourcesContainer *tileSourcesContainer;

@property (nonatomic, retain) id <RMTileSource> tileSource; // the first tile source, for backwards compatibility
@property (nonatomic, retain) NSArray *tileSources;

- (void)addTileSource:(id <RMTileSource>)tileSource;
- (void)addTileSource:(id<RMTileSource>)tileSource atIndex:(NSUInteger)index;

- (void)removeTileSource:(id <RMTileSource>)tileSource;
- (void)removeTileSourceAtIndex:(NSUInteger)index;

- (void)moveTileSourceAtIndex:(NSUInteger)fromIndex toIndex:(NSUInteger)toIndex;

- (void)setHidden:(BOOL)isHidden forTileSource:(id <RMTileSource>)tileSource;
- (void)setHidden:(BOOL)isHidden forTileSourceAtIndex:(NSUInteger)index;

- (void)reloadTileSource:(id <RMTileSource>)tileSource;
- (void)reloadTileSourceAtIndex:(NSUInteger)index;

/** @name Managing Tile Caching Behavior */

#pragma mark - Cache

//  Clear all images from the #tileSource's caching system.
-(void)removeAllCachedImages;

/** @name Converting Map Coordinates */

#pragma mark - Conversions

// projections to convert from latitude/longitude to meters, from projected meters to tile coordinates
@property (nonatomic, readonly) RMProjection *projection;
@property (nonatomic, readonly) id <RMMercatorToTileProjection> mercatorToTileProjection;

- (CGPoint)projectedPointToPixel:(RMProjectedPoint)projectedPoint;
- (CGPoint)coordinateToPixel:(CLLocationCoordinate2D)coordinate;

- (RMProjectedPoint)pixelToProjectedPoint:(CGPoint)pixelCoordinate;
- (CLLocationCoordinate2D)pixelToCoordinate:(CGPoint)pixelCoordinate;

- (RMProjectedPoint)coordinateToProjectedPoint:(CLLocationCoordinate2D)coordinate;
- (CLLocationCoordinate2D)projectedPointToCoordinate:(RMProjectedPoint)projectedPoint;

- (RMProjectedSize)viewSizeToProjectedSize:(CGSize)screenSize;
- (CGSize)projectedSizeToViewSize:(RMProjectedSize)projectedSize;

- (CLLocationCoordinate2D)normalizeCoordinate:(CLLocationCoordinate2D)coordinate;
- (RMTile)tileWithCoordinate:(CLLocationCoordinate2D)coordinate andZoom:(int)zoom;

- (RMSphericalTrapezium)latitudeLongitudeBoundingBoxForTile:(RMTile)aTile;

#pragma mark -
#pragma mark User Location

/** Sets the mode used to track the user location with optional animation.
 
 @param mode The mode used to track the user location. Possible values are described in “RMUserTrackingMode.”
 @param animated If YES, the change from the current mode to the new mode is animated; otherwise, it is not. This parameter affects only tracking mode changes. Changes to the user location or heading are always animated.
*/
- (void)setUserTrackingMode:(RMUserTrackingMode)mode animated:(BOOL)animated;

@end
