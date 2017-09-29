#!/bin/bash

if (cd server ; make) then
  if make ; then
    ./bin/app
  fi
fi


