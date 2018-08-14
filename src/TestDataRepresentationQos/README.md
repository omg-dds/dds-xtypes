# About this Module
This module tests whether different data representations are compatible with each other by creating DataReaders and DataWriters with different Data Representation QOS and check if discovery takes place.


## DataRepresentationQosPolicy Matching Rules <a id="3"></a>

This test tests the compatibility matrix for the DataRepresentationQosPolicy.

|DataWriter offered<br/><br/>DataRepresentationId_t<br/><br/>|DataReader requested<br/><br/>DataRepresentationId_t<br/><br/>|Compatible?<br/><br/><br/>|
|-------|-------|-------|
|XCDR|XCDR|Yes|
|XCDR|XCDR2|No|
|XCDR|XCDR and XCDR2|Yes|
|XCDR2|XCDR|No|
|XCDR2|XCDR2|Yes|
|XCDR2|XCDR and XCDR2|Yes|