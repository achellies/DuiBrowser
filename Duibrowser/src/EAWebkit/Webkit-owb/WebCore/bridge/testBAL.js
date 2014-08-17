myInterface.logMessage ("Starting test");

myInterface.logMessage ("Testing properties:");
myInterface.logMessage ("myInterface.doubleValue = " + myInterface.doubleValue);
myInterface.logMessage ("myInterface.intValue = " + myInterface.intValue);
myInterface.logMessage ("myInterface.stringValue = " + myInterface.stringValue);
myInterface.logMessage ("myInterface.booleanValue = " + myInterface.booleanValue);
myInterface.logMessage ("myInterface.nullValue = " + myInterface.nullValue);
myInterface.logMessage ("myInterface.undefinedValue = " + myInterface.undefinedValue);

myInterface.logMessage ("myInterface.setInt_(666) = " + myInterface.setIntValue(666));
myInterface.logMessage ("myInterface.getInt() = " + myInterface.getIntValue());
myInterface.logMessage ("myInterface.getString() = " + myInterface.getStringValue());
myInterface.logMessage ("setting myInterface.intValue = 777");
myInterface.intValue = 777;
myInterface.logMessage ("myInterface.intValue = " + myInterface.intValue);
var test = myInterface.myTest;
test.log("Ajout d'un nouvel objet!!!" );
