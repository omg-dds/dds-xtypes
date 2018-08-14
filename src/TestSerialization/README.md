# About this Module
This module focuses on testing xtype features. Different from `TestExtensibility` module, this module verifies that received data is correct.
Testing Procedure:

1. Create DataReader/DataWriter pair.
2. DataWriter writes a Data sample.
3. DataReader reads a data sample.
4. Verify that data received is the same as expected.

# TestSerialization Test Plan

5 [DataRepresentation Serialization/Deserialization Rules](#5)

5.1 [Test procedure](#5.1)

5.2 [Scenarios](#5.2)

5.2.1 [Extensibility Kind (#SER1)](#5.2.1)

5.2.2 [Add Member at the End of a Type with Default Value(#SER2)](#5.2.2)

5.2.3 [Add Member at the End of Nested Type With Default Value(#SER3)](#5.2.3)

5.2.4 [Add Member in the Middle of a Type With Default Value(#SER4)](#5.2.4)

5.2.5 [Type Refactoring Using Inheritance(#SER5)](#5.2.5)

5.2.6 [Different Maximum String Length in Publisher and Subscriber Type with try_construct TRIM(#SER6)](#5.2.6)

5.2.7 [Different Maximum Sequence Length in Publisher and Subscriber Type with try_construct DISCARD(#SER7)](#5.2.7)

5.2.8 [Reorder Members in a Union Type (#SER8)](#5.2.8)

5.2.9 [Add Member with Non-Default Label to a Union Type without Default Label(#SER9)](#5.2.9)

5.2.10 [Add Member with Out-of-Range Non-Default Label to a Union Type without Default. Discriminator uses try_construct DEFAULT(#SER10)](#5.2.10)

5.2.11 [Add Member with Default Label to a Union Type without Default Label](#5.2.11)

5.2.12 [Setting optional member to NULL](#5.2.12)

5.2.13 [Making an existing member optional](#5.2.13)

# 5 DataRepresentation Serialization/Deserialization Rules <a id="5"></a>

This feature test tests the serialization and deserialization rules for XCDR v1 and XCDR v2. 

The test only validates scenarios in which the Type of the DataWriter and DataReader are compatible. Type compatibility rules are tested separately here.

Like in Type Compatibility, the test uses different "versions" of two basic data-types: ShapeType and ParameterType being one of them a struct and the other a union. These variations are obtained by adding, removing or reordering different attributes. They are also obtained by declaring different extensibility kinds for the data type (Final, Appendable [the default], and Mutable). These types are declared in the [shapes.idl](../../idl/shapes.idl) file.

## 5.1 Test procedure <a id="5.1"></a>

  * For each one of the rows in the scenario tables below, create a Pair (DataWriter, DataReader) on the same Topic and registered type name
  * DataWriter configuration:
  * Type is Publisher Type
  * DataRepresentationQosPolicy.value is Representation Id
  * DataReader configuration:
  * Type is Subscriber Type
  * DataRepresentationQosPolicy.value is Representation Id
  * Wait until discovery happens
  * DataWriter sends "Sent Sample"
  * Check that DataReader receives "Received Sample" or nothing is the sample is dropped. When checking that samples are not received, wait for a timeout before doing the check.

## 5.2 Scenarios <a id="5.2"></a>

To reduce the number of rows in a scenario table, the Representation Id column may have multiple values. Example:

| Publisher Type | Subscriber Type | Representation Id   |
|---|---|---|
| Shape1Final | Shape1Final | V1 and V2   |
  
The previous table is equivalent to:

 

| Publisher Type | Subscriber Type | Representation Id   |
|---|---|---|
| Shape1Final | Shape1Final | V1   |
| Shape1Final | Shape1Final | V2   |
  
Also, to reduce the amount of information in a row, we have extracted the Published and Received Samples in a separate table. For example:

| Publisher Type | Subscriber Type | Representation Id   |
|---|---|---|
| Shape1Final | Shape1Final | V1 and V2   |
  
#### 

|Published Sample| Received Sample  |
|---|---|
|<code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED",<br/>&nbsp;&nbsp;&nbsp;&nbsp;"REDA&nbsp;SHAPE&nbsp;DESC"<br/>&nbsp;&nbsp;&nbsp;&nbsp;{"Tag_1"},<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{1,5},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{10,15},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{20,25}<br/>&nbsp;&nbsp;&nbsp;&nbsp;},<br/>&nbsp;&nbsp;&nbsp;&nbsp;10<br/>};<code/>|<code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED",<br/>&nbsp;&nbsp;&nbsp;&nbsp;"REDA&nbsp;SHAPE&nbsp;DESC",<br/>&nbsp;&nbsp;&nbsp;&nbsp;{"Tag_1"},<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{1,5},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{10,15},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{20,25}<br/>&nbsp;&nbsp;&nbsp;&nbsp;},<br/>&nbsp;&nbsp;&nbsp;&nbsp;10<br/>};</code>
  
The previous two tables are equivalent to:

| Publisher Type | Subscriber Type | Representation Id | Published Sample | Received Sample |  
|---|---|---|---|---|
| Shape1Final | Shape1Final | V1 |<code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED",<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED&nbsp;SHAPE&nbsp;DESC",<br/>&nbsp;&nbsp;&nbsp;&nbsp;{"Tag_1"},<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{1,5},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{10,15},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{20,25}<br/>&nbsp;&nbsp;&nbsp;&nbsp;},<br/>&nbsp;&nbsp;&nbsp;&nbsp;10<br/>};</code>|<code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED",<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED&nbsp;SHAPE&nbsp;DESC",<br/>&nbsp;&nbsp;&nbsp;&nbsp;{"Tag_1"},<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{1,5},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{10,15},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{20,25}<br/>&nbsp;&nbsp;&nbsp;&nbsp;},<br/>&nbsp;&nbsp;&nbsp;&nbsp;10<br/>};</code>|
|Shape1Final | Shape1Final | V2 |<code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED",<br/>&nbsp;&nbsp;&nbsp;&nbsp;"REDA&nbsp;SHAPE&nbsp;DESC",<br/>&nbsp;&nbsp;&nbsp;&nbsp;{"Tag_1"},<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{1,5},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{10,15},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{20,25}&nbsp;<br/>&nbsp;&nbsp;&nbsp;&nbsp;},<br/>&nbsp;&nbsp;&nbsp;&nbsp;10<br/>};</code>|{<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED",<br/>&nbsp;&nbsp;&nbsp;&nbsp;"REDA&nbsp;SHAPE&nbsp;DESC",<br/>&nbsp;&nbsp;&nbsp;&nbsp;{"Tag_1"},<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{1,5},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{10,15},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{20,25}<br/>&nbsp;&nbsp;&nbsp;&nbsp;},<br/>&nbsp;&nbsp;&nbsp;&nbsp;10<br/>};|
  
### 5.2.1 Extensibility Kind (#SER1) <a id="5.2.1"></a>

| TestCode | Publisher Type | Subscriber Type |Representation Id  
| ---|---|---|---  |
| #SER1A | Shape1Final | Shape1Final | V1 and V2  
| #SER1B | Shape1Appendable | Shape1Appendable | V1 and V2 |
| #SER1C | Shape1Appendable | Shape1Default| V1 and V2 | 
| #SER1D | Shape1Mutable | Shape1Mutable | V1 and V2  |
  
#### 

Published Sample | Received Sample  
---|---
<code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED",<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED&nbsp;SHAPE&nbsp;DESC",<br/>&nbsp;&nbsp;&nbsp;&nbsp;{"Tag_1"},<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{1,5},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{10,15},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{20,25}<br/>&nbsp;&nbsp;&nbsp;&nbsp;},<br/>&nbsp;&nbsp;&nbsp;&nbsp;10<br/>};</code>|<code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED",<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED&nbsp;SHAPE&nbsp;DESC",<br/>&nbsp;&nbsp;&nbsp;&nbsp;{"Tag_1"},<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{1,5},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{10,15},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{20,25}<br/>&nbsp;&nbsp;&nbsp;&nbsp;},<br/>&nbsp;&nbsp;&nbsp;&nbsp;10<br/>};</code<
  
### 5.2.2 Add Member at the End of a Type with Default Value(#SER2) <a id="5.2.2"></a>

TypeConsistencEnforcementQosPolicy.prevent_type_widening set to 0

| TestCode   | Publisher Type   | Subscriber Type | Representation Id |
|------------|------------------|-----------------|-------------------|
| #SER2A | Shape1Appendable | Shape2Appendable | V1 and V2  |
| #SER2B | Shape1Mutable | Shape2Mutable | V1 and V2 |
| #SER2C | Shape2Appendable | Shape1Appendable |V1 and V2  |
| #SER2D | Shape2Mutable | Shape1Mutable | V1 and V2  |

#### 

| Publishing Type   | Subscribing Type   | Published Sample   | Received Sample |
|-------------------|--------------------|--------------------|-----------------|
| Shape1.. | Shape2.. |<code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED",<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED&nbsp;SHAPE&nbsp;DESC",<br/>&nbsp;&nbsp;&nbsp;&nbsp;{"Tag_1"},<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{1,5},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{10,15},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{20,25}<br/>&nbsp;&nbsp;&nbsp;&nbsp;},<br/>&nbsp;&nbsp;&nbsp;&nbsp;10,<br/>};</code>|<code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED",<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED&nbsp;SHAPE&nbsp;DESC",<br/>&nbsp;&nbsp;&nbsp;&nbsp;{"Tag_1"},<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{1,5},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{10,15},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{20,25}<br/>&nbsp;&nbsp;&nbsp;&nbsp;},<br/>&nbsp;&nbsp;&nbsp;&nbsp;10,<br/>&nbsp;&nbsp;&nbsp;&nbsp;90<br/>};</code>|
| Shape2.. | Shape1.. | <code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED",<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED&nbsp;SHAPE&nbsp;DESC",<br/>&nbsp;&nbsp;&nbsp;&nbsp;{"Tag_1"},<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{1,5},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{10,15},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{20,25}<br/>&nbsp;&nbsp;&nbsp;&nbsp;},<br/>&nbsp;&nbsp;&nbsp;&nbsp;10,<br/>&nbsp;&nbsp;&nbsp;&nbsp;180<br/>};</code>|<code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED",<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED&nbsp;SHAPE&nbsp;DESC",<br/>&nbsp;&nbsp;&nbsp;&nbsp;{"Tag_1"},<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{1,5},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{10,15},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{20,25}<br/>&nbsp;&nbsp;&nbsp;&nbsp;},<br/>&nbsp;&nbsp;&nbsp;&nbsp;10,<br/>};&nbsp;&nbsp;</code>|  
### 5.2.3 Add Member at the End of Nested Type With Default Value(#SER3) <a id="5.2.3"></a>

TypeConsistencEnforcementQosPolicy.prevent_type_widening set to 0

| TestCode | Publisher Type   | Subscriber Type   | Representation Id |
|----------|------------------|-------------------|-------------------| 
| #SER3A   | Shape1Appendable | Shape3Appendable  | V2                |
| #SER3B   | Shape1Mutable    | Shape3Mutable     | V1 and V2         |
| #SER3C   | Shape3Appendable | Shape1Appendable  | V2                |
| #SER3D   | Shape3Mutable    | Shape1Mutable     | V1 and V2         |
  
#### 

| Publishing Type   | Subscribing Type   | Published Sample   | Received Sample |
|-------------------|--------------------|--------------------|-----------------|
| Shape1.. | Shape3.. |<code>{&nbsp;&nbsp;&nbsp;&nbsp;"RED",<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED&nbsp;SHAPE&nbsp;DESC",<br/>&nbsp;&nbsp;&nbsp;&nbsp;{"Tag_1"},<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{1,5},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{10,15},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{20,25}<br/>&nbsp;&nbsp;&nbsp;&nbsp;},<br/>&nbsp;&nbsp;&nbsp;&nbsp;10,<br/>};</code>|<code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED",<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED&nbsp;SHAPE&nbsp;DESC",<br/>&nbsp;&nbsp;&nbsp;&nbsp;{"Tag_1"},<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{1,5,10},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{10,15,10},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{20,25,10}<br/>&nbsp;&nbsp;&nbsp;&nbsp;},<br/>&nbsp;&nbsp;&nbsp;&nbsp;10<br/>};</code>
| Shape3.. | Shape1.. |<code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED",<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED&nbsp;SHAPE&nbsp;DESC",<br/>&nbsp;&nbsp;&nbsp;&nbsp;{"Tag_1"},<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{1,5,20},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{10,15,90},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{20,25,33}<br/>&nbsp;&nbsp;&nbsp;&nbsp;},<br/>&nbsp;&nbsp;&nbsp;&nbsp;10<br/>};</code>|<code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED",<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED&nbsp;SHAPE&nbsp;DESC",<br/>&nbsp;&nbsp;&nbsp;&nbsp;{"Tag_1"},<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{1,5},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{10,15},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{20,25}<br/>&nbsp;&nbsp;&nbsp;&nbsp;},<br/>&nbsp;&nbsp;&nbsp;&nbsp;10,<br/>};&nbsp;&nbsp;
</code>
  
### 5.2.4 Add Member in the Middle of a Type With Default Value(#SER4) <a id="5.2.4"></a>

TypeConsistencEnforcementQosPolicy.prevent_type_widening set to 0

| TestCode | Publisher Type          | Subscriber Type         | Representation Id |
|----------|-------------------------|-------------------------|-------------------|
| #SER4A   | Shape1MutableExplicitID |Shape4MutableExplicitID  | V1 and V2         |
| #SER4B   | Shape4MutableExplicitID | Shape1MutableExplicitID | V1 and V2         |
  
#### 

| Publishing Type   | Subscribing Type   | Published Sample   | Received Sample |
---|---|---|--- 
| Shape1.. | Shape4.. |<code>{<br>&nbsp;&nbsp;&nbsp;&nbsp;"RED",<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED&nbsp;SHAPE&nbsp;DESC",<br/>&nbsp;&nbsp;&nbsp;&nbsp;{"Tag_1"},<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{1,5},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{10,15},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{20,25}<br/>&nbsp;&nbsp;&nbsp;&nbsp;},<br/>&nbsp;&nbsp;&nbsp;&nbsp;10,<br/>};</code> |<code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED",<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED&nbsp;SHAPE&nbsp;DESC",<br/>&nbsp;&nbsp;&nbsp;&nbsp;{"Tag_1"},<br/>&nbsp;&nbsp;&nbsp;&nbsp;90,<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{1,5},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{10,15},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{20,25}<br/>&nbsp;&nbsp;&nbsp;&nbsp;},<br/>&nbsp;&nbsp;&nbsp;&nbsp;10<br/>};</code> |
| Shape4.. | Shape1.. |<code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED",<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED&nbsp;SHAPE&nbsp;DESC",<br/>&nbsp;&nbsp;&nbsp;&nbsp;{"Tag_1"},<br/>&nbsp;&nbsp;&nbsp;&nbsp;22,<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{1,5},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{10,15},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{20,25}<br/>&nbsp;&nbsp;&nbsp;&nbsp;},<br/>&nbsp;&nbsp;&nbsp;&nbsp;10<br/>};</code> | <code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED",<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED&nbsp;SHAPE&nbsp;DESC",<br/>&nbsp;&nbsp;&nbsp;&nbsp;{"Tag_1"},<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{1,5},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{10,15},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{20,25}<br/>&nbsp;&nbsp;&nbsp;&nbsp;},<br/>&nbsp;&nbsp;&nbsp;&nbsp;10<br/>};</code>|

  
### 5.2.5 Type Refactoring Using Inheritance(#SER5) <a id="5.2.5"></a>

| TestCode   | Publisher Type   | Subscriber Type   |  Representation Id |
---|---|---|---
| #SER5A | Shape2Final | Shape5Final | V1 and V2  |
| #SER5B | Shape2Appendable | Shape5Appendable |V1 and V2 |
| #SER5C | Shape2Mutable | Shape5Mutable | V1 and V2 |
| #SER5D | Shape5Final | Shape2Final | V1 and V2  |
| #SER5E | Shape5Appendable | Shape2Appendable | V1 and V2  |
| #SER5F | Shape5Mutable | Shape2Mutable | V1 and V2  |
  
#### 

| Publishing Type   | Subscribing Type   | Published Sample   |  |
---|---|---|---  
| Shape2.. | Shape5.. |<code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED",<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED&nbsp;SHAPE&nbsp;DESC",<br/>&nbsp;&nbsp;&nbsp;&nbsp;{"Tag_1"},<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{1,5}[ad],<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{10,15},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{20,25}<br/>&nbsp;&nbsp;&nbsp;&nbsp;},<br/>&nbsp;&nbsp;&nbsp;&nbsp;10,<br/>&nbsp;&nbsp;&nbsp;&nbsp;90[ae][af][ag],<br/>};</code> |<code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"RED",<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"RED&nbsp;SHAPE&nbsp;DESC",<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{"Tag_1"},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{1,5},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{10,15},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{20,25}<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;10<br/>&nbsp;&nbsp;&nbsp;&nbsp;},<br/>&nbsp;&nbsp;&nbsp;&nbsp;90<br/>};&nbsp;</code> 
| Shape5.. | Shape2.. | <code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"RED",<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"RED&nbsp;SHAPE&nbsp;DESC",<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{"Tag_1"},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{1,5},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{10,15},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{20,25}<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;10<br/>&nbsp;&nbsp;&nbsp;&nbsp;},<br/>&nbsp;&nbsp;&nbsp;&nbsp;90<br/>};</code>|<code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED",<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED&nbsp;SHAPE&nbsp;DESC",<br/>&nbsp;&nbsp;&nbsp;&nbsp;{"Tag_1"},<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{1,5},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{10,15},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{20,25}<br/>&nbsp;&nbsp;&nbsp;&nbsp;},<br/>&nbsp;&nbsp;&nbsp;&nbsp;10,<br/>&nbsp;&nbsp;&nbsp;&nbsp;90,<br/>};</code>
  
### 5.2.6 Different Maximum String Length in Publisher and Subscriber Type with try_construct TRIM(#SER6) <a id="5.2.6"></a>

TypeConsistencyEnforcement.ignore_string_bounds to 1

| TestCode   | Publisher Type   | Subscriber Type   |  Representation Id |
---|---|---|---
| #SER6A | Shape1Final | Shape6Final | V1 and V2  
| #SER6B | Shape1Appendable | Shape6Appendable | V1 and V2  |
| #SER6C | Shape1Mutable | Shape6Mutable | V1 and V2 |
| #SER6D | Shape6Final | Shape1Final | V1 and V2 |
| #SER6E | Shape6Appendable | Shape1Appendable | V1 and V2  |
| #SER6D | Shape6Mutable | Shape1Mutable | V1 and V2 |
  
#### 

| Publishing Type   | Subscribing Type   | Published Sample   | Received Sample |
---|---|---|---  
| Shape1.. | Shape6.. |<code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED",<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED&nbsp;SHAPE&nbsp;DESC",<br/>&nbsp;&nbsp;&nbsp;&nbsp;{"Tag_1"},<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{1,5},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{10,15},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{20,25}<br/>&nbsp;&nbsp;&nbsp;&nbsp;},<br/>&nbsp;&nbsp;&nbsp;&nbsp;10<br/>};</code> |<code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED",<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED&nbsp;SHAPE",<br/>&nbsp;&nbsp;&nbsp;&nbsp;{"Tag_1"},<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{1,5},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{10,15},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{20,25},<br/>&nbsp;&nbsp;&nbsp;&nbsp;},<br/>&nbsp;&nbsp;&nbsp;&nbsp;10<br/>};&nbsp;&nbsp;</code>
| Shape6.. | Shape1.. | <code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED",<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED&nbsp;SHAPE",<br/>&nbsp;&nbsp;&nbsp;&nbsp;{"Tag_1"},<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{1,5},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{10,15},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{20,25}<br/>&nbsp;&nbsp;&nbsp;&nbsp;},<br/>&nbsp;&nbsp;&nbsp;&nbsp;10<br/>};</code> |<code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED",<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED&nbsp;SHAPE",<br/>&nbsp;&nbsp;&nbsp;&nbsp;{"Tag_1"},<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{1,5},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{10,15},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{20,25}<br/>&nbsp;&nbsp;&nbsp;&nbsp;},<br/>&nbsp;&nbsp;&nbsp;&nbsp;10,<br/>};&nbsp;&nbsp;</code>
  
### 5.2.7 Different Maximum Sequence Length in Publisher and Subscriber Type with try_construct DISCARD(#SER7) <a id="5.2.7"></a>

TypeConsistencyEnforcement.ignore_sequence_bounds to 1

| TestCode   | Publisher Type   | Subscriber Type   |  Representation Id |
---|---|---|---  
| #SER7A | Shape1Final | Shape6Final | V1 and V2 |
| #SER7B | Shape1Appendable | Shape6Appendable | V1 and V2 |
| #SER7C | Shape1Mutable | Shape6Mutable | V1 and V2 |
| #SER7D | Shape6Final | Shape1Final | V1 and V2  |
| #SER7E | Shape6Appendable | Shape1Appendable | V1 and V2 |
| #SER7F | Shape6Mutable | Shape1Mutable | V1 and V2 |
  
#### 

| Publishing Type   | Subscribing Type   | Published Sample   | Received Sample |
---|---|---|---  
| Shape1.. | Shape6.. | <code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED",<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED&nbsp;SHAPE&nbsp;DESC",<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"Tag_1",<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"Tag_2",<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"Tag_3"<br/>&nbsp;&nbsp;&nbsp;&nbsp;},<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{1,5},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{10,15},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{20,25}<br/>&nbsp;&nbsp;&nbsp;&nbsp;},<br/>&nbsp;&nbsp;&nbsp;&nbsp;10<br/>};</code>| Sample is dropped 
| Shape6.. | Shape1.. | <code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED",<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED&nbsp;SHAPE&nbsp;DESC",<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"Tag_1",<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"Tag_2"<br/>&nbsp;&nbsp;&nbsp;&nbsp;},<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{1,5},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{10,15},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{20,25}<br/>&nbsp;&nbsp;&nbsp;&nbsp;},<br/>&nbsp;&nbsp;&nbsp;&nbsp;10<br/>};</code>| <code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED",<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED&nbsp;SHAPE&nbsp;DESC",<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"Tag_1",<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"Tag_2"<br/>&nbsp;&nbsp;&nbsp;&nbsp;},<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{1,5},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{10,15},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{20,25}<br/>&nbsp;&nbsp;&nbsp;&nbsp;},<br/>&nbsp;&nbsp;&nbsp;&nbsp;10<br/>};</code>
  
### 5.2.8 Reorder Members in a Union Type (#SER8) <a id="5.2.8"></a>

| TestCode   | Publisher Type   | Subscriber Type   |  Representation Id |
---|---|---|---  
| #SER8A | Parameter1Final | Parameter3Final | V1 and V2  |
| #SER8B | Parameter1Appendable | Parameter3Appendable | V1 and V2  |
| #SER8C | Parameter1Mutable | Parameter3Mutable | V1 and V2 |
| #SER8D | Parameter3Final | Parameter1Final | V1 and V2  |
| #SER8E | Parameter3Appendable | Parameter1Appendable | V1 and V2  |
| #SER8F | Parameter3Mutable | Parameter1Mutable | V1 and V2  |
  
#### 

Published Sample | Received Sample  
---|--- 
<code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;_d:&nbsp;USHORT_PARAM,<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;val_us:&nbsp;34<br/>&nbsp;&nbsp;&nbsp;&nbsp;}<br/>};</code>|<code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;_d:&nbsp;USHORT_PARAM,<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;val_us:&nbsp;34<br/>&nbsp;&nbsp;&nbsp;&nbsp;}<br/>};</code>
  
### 5.2.9 Add Member with Non-Default Label to a Union Type without Default Label(#SER9) <a id="5.2.9"></a>

| TestCode   | Publisher Type   | Subscriber Type   |  Representation Id|
---|---|---|---
| #SER9A | Parameter1Appendable | Parameter2Appendable | V1 and V2 |
| #SER9B | Parameter1Mutable | Parameter2Mutable | V1 and V2 |
| #SER9C | Parameter2Appendable | Parameter1Appendable | V1 and V2 |
| #SER9D | Parameter2Mutable | Parameter1Mutable | V1 and V2 |
  
#### 

| Publishing Type   | Subscribing Type   | Published Sample   | Representation Id |
---|---|---|---
| Parameter1.. | Parameter2.. | <code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;_d:&nbsp;USHORT_PARAM,<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;val_us:&nbsp;34<br/>&nbsp;&nbsp;&nbsp;&nbsp;}<br/>};</code> | <code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;_d:&nbsp;USHORT_PARAM,<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;val_us:&nbsp;34<br/>&nbsp;&nbsp;&nbsp;&nbsp;}<br/>};</code>
| Parameter2.. | Parameter1.. | <code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;_d:&nbsp;LONG_PARAM,<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;val_l:&nbsp;34<br/>&nbsp;&nbsp;&nbsp;&nbsp;}<br/>};</code>|<code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;_d:&nbsp;LONG_PARAM,<br/>};</code> 
  
### 5.2.10 Add Member with Out-of-Range Non-Default Label to a Union Type without Default. Discriminator uses try_construct DEFAULT(#SER10) <a id="5.2.10"></a>

| TestCode   | Publisher Type   | Subscriber Type   |  Representation Id |
---|---|---|---    
| #SER10A | Parameter1Appendable | Parameter7Appendable | V1 and V2  |
| #SER10B | Parameter1Mutable | Parameter7Mutable | V1 and V2 |
| #SER10C | Parameter7Appendable | Parameter1Appendable | V1 and V2  |
| #SER10D | Parameter7Mutable | Parameter1Mutable | V1 and V2  |
  
#### 

| Publishing Type   | Subscribing Type   | Published Sample   | Representation Id |
---|---|---|---    
| Parameter1.. | Parameter7.. | <code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;_d:&nbsp;USHORT_PARAM,<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;val_us:&nbsp;34<br/>&nbsp;&nbsp;&nbsp;&nbsp;}<br/>};</code> | <code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;_d:&nbsp;USHORT_PARAM,<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;val_us:&nbsp;34<br/>&nbsp;&nbsp;&nbsp;&nbsp;}<br/>};</code> 
|Parameter7..|Parameter1.|<code><br/>&nbsp;&nbsp;&nbsp;&nbsp;_d:&nbsp;LONGLONG_PARAM,<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;val_ll:&nbsp;128<br/>&nbsp;&nbsp;&nbsp;&nbsp;}<br/>};</code>|<code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;_d:&nbsp;SHORT_PARAM,<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;val_s:&nbsp;0<br/>&nbsp;&nbsp;&nbsp;&nbsp;}<br/>};</code>
  
### 5.2.11 Add Member with Default Label to a Union Type without Default Label (#SER11) <a id="5.2.11"></a>

| TestCode   | Publisher Type   | Subscriber Type   |  Representation Id |
---|---|---|--- 
| #SER11A | Parameter1Appendable | Parameter4Appendable | V1 and V2 |
| #SER11B | Parameter1Mutable | Parameter4Mutable | V1 and V2 |
| #SER11C | Parameter4Appendable | Parameter1Appendable | V1 and V2 |
| #SER11D | Parameter4Mutable | Parameter1Mutable | V1 and V2 |
  
#### 

| Publishing Type   | Subscribing Type   | Published Sample   | Representation Id |
---|---|---|---  
| Parameter1.. | Parameter4.. | <code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;_d:&nbsp;USHORT_PARAM,<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;val_us:&nbsp;34<br/>&nbsp;&nbsp;&nbsp;&nbsp;}<br/>};</code> | <code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;_d:&nbsp;USHORT_PARAM,<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;val_us:&nbsp;34<br/>&nbsp;&nbsp;&nbsp;&nbsp;}<br/>};&nbsp;&nbsp;</code>
| Parameter4.. | Parameter1.. | <code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;_d:&nbsp;LONG_PARAM,<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;val_l:&nbsp;34<br/>&nbsp;&nbsp;&nbsp;&nbsp;}<br/>};</code> | <code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;_d:&nbsp;LONG_PARAM,<br/>};</code>
  
### 5.2.12 Setting optional member to NULL(#SER12) <a id="5.2.12"></a>

| TestCode   | Publisher Type   | Subscriber Type   | Representation Id |
---|---|---|---  
| #SER12A | Shape1Final | Shape1Final | V1 and V2 |
| #SER12B | Shape1Appendable | Shape1Appendable | V1 and V2 |
| #SER12C | Shape1Mutable | Shape1Mutable | V1 and V2  |
  
#### 

| Publishing Type   | Subscribing Type   | Published Sample   | Received Sample |
|---|---|---|---|
| Shape1.. | Shape1.. |<code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED",<br/>&nbsp;&nbsp;&nbsp;&nbsp;"NULL",<br/>&nbsp;&nbsp;&nbsp;&nbsp;{"Tag_1"},<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{1,5},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{10,15},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{20,25}<br/>&nbsp;&nbsp;&nbsp;&nbsp;},<br/>&nbsp;&nbsp;&nbsp;&nbsp;10<br/>};</code>|<code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED",<br/>&nbsp;&nbsp;&nbsp;&nbsp;"NULL",<br/>&nbsp;&nbsp;&nbsp;&nbsp;{"Tag_1"},<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{1,5},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{10,15},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{20,25},<br/>&nbsp;&nbsp;&nbsp;&nbsp;},<br/>&nbsp;&nbsp;&nbsp;&nbsp;10<br/>};</code>|
  
### 5.2.13 Making an existing member optional(#SER13) <a id="5.2.13"></a>

| TestCode | Publisher Type                   | Subscriber Type  |  Representation Id | 
|----------|----------------------------------|------------------|--------------------|
| #SER13A  | Shape1Mutable (optional not set) | Shape11Mutable   | V1 and V2          |
| #SER13B  | Shape1Mutable (optional set)     | Shape11Mutable   | V1 and V2          |
| #SER13C  | Shape11Mutable                   | Shape1Mutable    | V1 and V2          |
  
#### 

| Publishing Type   | Subscribing Type   | Published Sample   | Representation Id |
|---|---|---|---|
|Shape1.. (optional not set) | Shape11.. |<code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED",<br/>&nbsp;&nbsp;&nbsp;&nbsp;"NULL",<br/>&nbsp;&nbsp;&nbsp;&nbsp;{"Tag_1"},<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{1,5},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{10,15},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{20,25}<br/>&nbsp;&nbsp;&nbsp;&nbsp;},<br/>&nbsp;&nbsp;&nbsp;&nbsp;10<br/>};</code>|<code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED",<br/>&nbsp;&nbsp;&nbsp;&nbsp;"NULL",<br/>&nbsp;&nbsp;&nbsp;&nbsp;{"Tag_1"},<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{1,5},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{10,15},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{20,25},<br/>&nbsp;&nbsp;&nbsp;&nbsp;},<br/>&nbsp;&nbsp;&nbsp;&nbsp;10<br/>};</code>|
|Shape1.. (optional set) | Shape11.. | <code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED",<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED&nbsp;SHAPE&nbsp;DESC",<br/>&nbsp;&nbsp;&nbsp;&nbsp;{"Tag_1"},<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{1,5},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{10,15},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{20,25}<br/>&nbsp;&nbsp;&nbsp;&nbsp;},<br/>&nbsp;&nbsp;&nbsp;&nbsp;10<br/>};</code>| <code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED",<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED&nbsp;SHAPE&nbsp;DESC",<br/>&nbsp;&nbsp;&nbsp;&nbsp;{"Tag_1"},<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{1,5},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{10,15},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{20,25},<br/>&nbsp;&nbsp;&nbsp;&nbsp;},<br/>&nbsp;&nbsp;&nbsp;&nbsp;10<br/>};</code>|
| Shape 11 | Shape1  | <code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED",<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED&nbsp;SHAPE&nbsp;DESC",<br/>&nbsp;&nbsp;&nbsp;&nbsp;{"Tag_1"},<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{1,5},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{10,15},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{20,25}[ar][as]<br/>&nbsp;&nbsp;&nbsp;&nbsp;},<br/>&nbsp;&nbsp;&nbsp;&nbsp;10<br/>};</code>| <code>{<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED",<br/>&nbsp;&nbsp;&nbsp;&nbsp;"RED&nbsp;SHAPE&nbsp;DESC",<br/>&nbsp;&nbsp;&nbsp;&nbsp;{"Tag_1"},<br/>&nbsp;&nbsp;&nbsp;&nbsp;{<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{1,5},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{10,15},<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{20,25},<br/>&nbsp;&nbsp;&nbsp;&nbsp;},<br/>&nbsp;&nbsp;&nbsp;&nbsp;10<br/>};</code>|
  
# 5 Missing Scenarios <a id="5"></a>

  * Typedef

