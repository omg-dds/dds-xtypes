# About this Module
This module focus on xtype features.

Xtype allows DDS application with different types to communicate with each other. For example, one side might have an additional member at the end of type. The spec for xtype can be found [here](https://www.omg.org/spec/DDS-XTypes/About-DDS-XTypes/).

The testing procedure looks as follows:

1. Create a DataReader/DataWriter pair, possibly with different types.
2. DataWriter writes a sample.
3. DataReader tries to read a sample.
4. Check if data is present, or if the middleware treats two types as incompatible.

# TestExtensibility Test Plan


4 [Type Compatibility Rules](#4)

4.1 [Test procedure](#4.1)

4.2 [Scenarios](#4.2)

4.2.1 [Extensibility Kind](#4.2.1)

4.2.2 [Add Member at the End of a Type](#4.2.2)

4.2.3 [Add Member at the End of Nested Type](#4.2.3)

4.2.4 [Add Member in the Middle of a Type](#4.2.4)

4.2.5 [Type Refactoring Using Inheritance](#4.2.5)

4.2.6 [Different Maximum String Length in Publisher and Subscriber Type](#4.2.6)

4.2.7 [Different Maximum Sequence Length in Publisher and Subscriber Type](#4.2.7)

4.2.8 [Change the name of a member](#4.2.8)

4.2.9 [Change the type of a primitive member](#4.2.9)

4.2.10 [Change array length](#4.2.10)

4.2.11 [Reorder Members in a Union Type](#4.2.11)

4.2.12 [Add Member with Non-Default Label to a Union Type without Default Label](#4.2.12)

4.2.13 [Add Member with Default Label to a Union Type without Default Label](#4.2.13)

4.2.14 [Add Member with Non-Default Label to a Union Type with Default Label](#4.2.14)

4.2.15 [Make an existing member optional](#4.2.15)

# 4 Type Compatibility Rules <a id="4"></a>


The feature test is an extension of the test located in the GIT repository [OMG Interoperability Examples](https://www.google.com/url?q=https://github.com/omg-dds/dds-xtypes&sa=D&ust=1533927289318000).

The test uses different "versions" of two basic data-types: ShapeType, and ParameterType, being one of them a struct and the other a union. These variations are obtained by adding, removing or reordering different attributes. They are also obtained by declaring different extensibility kinds for the data type (Final, Appendable [the default], and Mutable). These types are declared in the [shapes.idl](../../idl/shapes.idl) file.

## 4.1 Test procedure <a id="4.1"></a>

  * For each one of the rows in the scenario tables below, create a Pair (DataWriter, DataReader) on the same Topic
  * DataWriter type is Publisher Type and offers XCDR2 representation Id 
  * DataReader type is Subscriber Type and requests XCDR2 representation Id
  * If the types are not compatible check that the DDS_InconsistentTopicStatus is updated and the DataWriter and DataReader do not discover each other
  * If the types are compatible check that the DataReader receives samples from the DataWriter. No need to check for sample content in this test. This will be done in the DataRepresentation Serialization/Deserialization rules tests.

## 4.2 Scenarios <a id="4.2"></a>

Unless stated otherwise by adding additional rows to the Scenarios tables, the compatibility results by exchanging Publisher and Subscriber Types are the same.

### 4.2.1 Extensibility Kind  <a id="4.2.1"></a>

|Publisher Type<br/><br/>|Subscriber Type<br/><br/>|Compatibility<br/><br/>|
|-------|-------|-------|
|Shape1Final|Shape1Final|Yes|
|Shape1Final|Shape1Appendable|No|
|Shape1Final|Shape1Mutable|No|
|Shape1Final|Shape1Default|No|
|Shape1Appendable|Shape1Appendable|Yes|
|Shape1Appendable|Shape1Mutable|No|
|Shape1Appendable|Shape1Default|Yes|
|Shape1Mutable|Shape1Mutable|Yes|
|Shape1Mutable|Shape1Default|No|
  
### 4.2.2 Add Member at the End of a Type  <a id="4.2.2"></a>

XTypes 1.1

**XTypes 1.2 with TypeConsistencEnforcementQosPolicy.prevent_type_widening set to 0 (default)**

|Publisher Type<br/><br/>|Subscriber Type<br/><br/>|Compatibility<br/><br/>|
|-------|-------|-------|
|Shape1Final<br/><br/>|Shape2Final<br/><br/>|Not Compatible<br/><br/>|
|Shape1Appendable<br/><br/>|Shape2Appendable<br/><br/>|Compatible<br/><br/>|
|Shape1Mutable<br/><br/>|Shape2Mutable<br/><br/>|Compatible<br/><br/>|
  
**XTypes 1.2 with   TypeConsistencEnforcementQosPolicy.prevent_type_widening  set to 1 (non default)**

|Publisher Type|Subscriber Type|Compatibility|
|-------|-------|-------|
|Shape1Final|Shape2Final|Not Compatible|
|Shape1Appendable|Shape2Appendable|Not Compatible|
|Shape1Mutable|Shape2Mutable|Not Compatible|
|Shape2Final|Shape1Final|Not Compatible|
|Shape2Appendable|Shape1Appendable|Compatible|
|Shape2Mutable|Shape1Mutable|Compatible|
  
### 4.2.3 Add Member at the End of Nested Type  <a id="4.2.3"></a>

V1 Encapsulation in DataWriter

|Publisher Type|Subscriber Type|Compatibility|
|-------|-------|-------|
|Shape1Final|Shape3Final|Not Compatible|
|Shape1Appendable|Shape3Appendable|Not Compatible|
|Shape1Mutable|Shape3Mutable|Compatible|
  
V2 Encapsulation on DataWriter

|Publisher Type|Subscriber Type|Compatibility|
|-------|-------|-------|
|Shape1Final|Shape3Final|Not Compatible|
|Shape1Appendable|Shape3Appendable|Compatible|
|Shape1Mutable|Shape3Mutable|Compatible|
  
### 4.2.4 Add Member in the Middle of a Type  <a id="4.2.4"></a>

|Publisher Type|Subscriber Type|Compatibility|
|-------|-------|-------|
|Shape1Final|Shape4Final|Not Compatible|
|Shape1Appendable|Shape4Appendable|Not Compatible|
|Shape1Mutable|Shape4Mutable|Not Compatible|
|Shape1MutableExplicitID|Shape4MutableExplicitID|Compatible|
  
### 4.2.5 Type Refactoring Using Inheritance <a id="4.2.5"></a>

|Publisher Type|Subscriber Type|Compatibility|
|-------|-------|-------|
|Shape2Final|Shape5Final|Compatible|
|Shape2Appendable|Shape5Appendable|Compatible|
|Shape2Mutable|Shape5Mutable|Compatible|
  
### 4.2.6 Different Maximum String Length in Publisher and Subscriber Type <a id="4.2.6"></a>

**XTypes 1.1 setting dds.type_consistency.ignore_string_bounds to 0 (default)**

**XTypes 1.2 setting TypeConsistencyEnforcement.ignore_string_bounds to 0 (non default)**

|Publisher Type|Subscriber Type|Compatibility|
|-------|-------|-------|
|Shape1Final|Shape6Final|Compatible|
|Shape1Appendable|Shape6Appendable|Compatible|
|Shape1Mutable|Shape6Mutable|Compatible|
|Shape6Final|Shape1Final|Not Compatible|
|Shape6Appendable|Shape1Appendable|Not Compatible|
|Shape6Mutable|Shape1Mutable|Not Compatible|
  
**XTypes 1.1 setting dds.type_consistency.ignore_sequence_bounds to 1 (non default)**

**XTypes 1.2 setting TypeConsistencyEnforcement.ignore_string_bounds to 1 (default)**

|Publisher Type|Subscriber Type|Compatibility|
|-------|-------|-------|
|Shape1Final|Shape6Final|Compatible|
|Shape1Appendable|Shape6Appendable|Compatible|
|Shape1Mutable|Shape6Mutable|Compatible|
|Shape6Final|Shape1Final|Compatible|
|Shape6Appendable|Shape1Appendable|Compatible|
|Shape6Mutable|Shape1Mutable|Compatible|
  
Note: dds.type_consistency.ignore_sequence_bounds is an RTI extension. The 1.1 spec per se did not allow ignoring string bounds.

### 4.2.7 Different Maximum Sequence Length in Publisher and Subscriber Type <a id="4.2.7"></a>

XTypes 1.1 setting dds.type_consistency.ignore_sequence_bounds to 0 (default)

XTypes 1.2 setting TypeConsistencyEnforcement.ignore_sequence_bounds to 0 (non default)

|Publisher Type|Subscriber Type|Compatibility|
|-------|-------|-------|
|Shape1Final|Shape7Final|Compatible|
|Shape1Appendable|Shape7Appendable|Compatible|
|Shape1Mutable|Shape7Mutable|Compatible|
|Shape7Final|Shape1Final|Not Compatible|
|Shape7Appendable|Shape1Appendable|Not Compatible|
|Shape7Mutable|Shape1Mutable|Not Compatible|

XTypes 1.1 setting dds.type_consistency.ignore_sequence_bounds to 1 (non default)

XTypes 1.2 setting TypeConsistencyEnforcement.ignore_sequence_bounds to 1 (default)

|Publisher Type|Subscriber Type|Compatibility|
|-------|-------|-------|
|Shape1Final|Shape7Final|Compatible|
|Shape1Appendable|Shape7Appendable|Compatible|
|Shape1Mutable|Shape7Mutable|Compatible|
|Shape7Final|Shape1Final|Compatible|
|Shape7Appendable|Shape1Appendable|Compatible|
|Shape7Mutable|Shape1Mutable|Compatible| 
  
Note: dds.type_consistency.ignore_sequence_bounds is an RTI extension. The 1.1 spec per se did not allow ignoring sequence bounds.

### 4.2.8 Change the name of a member <a id="4.2.8"></a>

XTypes 1.1 setting dds.type_consistency.ignore_member_names to 1 (non default)

XTypes 1.2 setting TypeConsistencyEnforcement.ignore_member_names to 1 (non default)

Note: dds.type_consistency.ignore_member_names is an RTI extension. The 1.1 spec per se did not allow ignoring member names.

| Publisher Type | Subscriber Type | Compatibility|
|---|---|---|
| Shape1Final | Shape8Final | Compatible   |
| Shape1Appendable | Shape8Appendable | Compatible   |
| Shape1Mutable | Shape8Mutable | Compatible   |
  
XTypes 1.1 setting dds.type_consistency.ignore_member_names to 0 (default)

XTypes 1.2 setting TypeConsistencyEnforcement.ignore_member_names to 0 (default)

| Publisher Type | Subscriber Type | Compatibility   |
|---|---|---|
| Shape1Final | Shape8Final | Not Compatible   |
| Shape1Appendable | Shape8Appendable | Not Compatible   |
| Shape1Mutable | Shape8Mutable | Not Compatible   |
  
### 4.2.9 Change the type of a primitive member <a id="4.2.9"></a>

| Publisher Type | Subscriber Type | Compatibility   |
|---|---|---|
| Shape1Final | Shape9Final | Not Compatible   |
| Shape1Appendable | Shape9Appendable | Not Compatible   |
| Shape1Mutable | Shape9Mutable | Not Compatible   |
  
### 4.2.10 Change array length <a id="4.2.10"></a>

| Publisher Type | Subscriber Type | Compatibility |
|----------------|-----------------|---------------|
| Shape1Final | Shape10Final | Not Compatible   |
| Shape1Appendable | Shape10Appendable | Not Compatible   |
| Shape1Mutable | Shape10Mutable | Not Compatible   |
| Shape10Final | Shape1Final | Not Compatible   |
| Shape10Appendable | Shape1Appendable | Not Compatible   |
| Shape10Mutable | Shape1Mutable | Not Compatible   |
  
### 4.2.11 Reorder Members in a Union Type <a id="4.2.11"></a>

| Publisher Type | Subscriber Type | Compatibility|
|---|---|---|
| Parameter1Final | Parameter3Final | Compatible |
| Parameter1Appendable | Parameter3Appendable | Compatible |
| Parameter1Mutable | Parameter3Mutable | Compatible|
  
### 4.2.12 Add Member with Non-Default Label to a Union Type without Default Label <a id="4.2.12"></a>

This can be done at the end or in the middle

| Publisher Type | Subscriber Type | Compatibility |
|---|---|---|
| Parameter1Final | Parameter2Final | Not Compatible|
| Parameter1Appendable | Parameter2Appendable | Compatible |
| Parameter1Mutable | Parameter2Mutable | Compatible |

### 4.2.13 Add Member with Default Label to a Union Type without Default Label[l][m] <a id="4.2.13"></a>

| Publisher Type | Subscriber Type | Compatibility |
|---|---|---|
| Parameter1Final | Parameter4Final | Not Compatible|
| Parameter1Appendable | Parameter4Appendable | Compatible   |
| Parameter1Mutable | Parameter4Mutable | Compatible|
  
### 4.2.14 Add Member with Non-Default Label to a Union Type with Default Label <a id="4.2.14"></a>

The result of this matching depends on whether or not the type for the new member is assignable from the default member type.

#### Type Assignable

| Publisher Type | Subscriber Type | Compatibility|
|---|---|---|
| Paramater4Final | Parameter5Final | Not Compatible|
| Parameter4Appendable | Parameter5Appendable | Compatible |
| Parameter4Mutable | Parameter5Mutable | Compatible|

#### Type Not Assignable

| Publisher Type | Subscriber Type | Compatibility   |
|---|---|---|
| Paramater4Final | Parameter6Final | Not Compatible   |
| Parameter4Appendable | Parameter6Appendable | Not Compatible   |
| Parameter4Mutable | Parameter6Mutable | Not Compatible   |
  
### 4.2.15 Make an existing member optional <a id="4.2.15"></a>

| Publisher Type | Subscriber Type | Compatibility   |
|---|---|---|
| Shape1Final | Shape11Final | Not Compatible   |
| Shape1Appendable | Shape11Appendable | Not Compatible   |
| Shape1Mutable | Shape11Mutable | Compatible |