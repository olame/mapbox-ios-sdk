#!/bin/sh

appledoc --project-name "MapBox iOS SDK" \
         --project-company MapBox \
         --company-id com.mapbox \
         --output Documentation \
         --ignore Map/FMDB \
         --ignore Map/GRMustache \
         --ignore build \
         --ignore Documentation \
         --ignore Map/NSUserDefaults+RouteMe.h \
         --clean-output \
         --no-install-docset \
         .
