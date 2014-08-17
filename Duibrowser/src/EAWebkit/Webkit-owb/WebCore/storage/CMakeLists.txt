IF (WEBKIT_USE_DATABASE)
    LIST (APPEND STORAGE_SRC
        storage/ChangeVersionWrapper.cpp
        storage/Database.cpp
        storage/DatabaseAuthorizer.cpp
        storage/DatabaseTask.cpp
        storage/DatabaseThread.cpp
        storage/DatabaseTracker.cpp
	storage/LocalStorage.cpp
	storage/LocalStorageArea.cpp
	storage/LocalStorageTask.cpp
	storage/LocalStorageThread.cpp
        storage/OriginQuotaManager.cpp
        storage/OriginUsageRecord.cpp
        storage/SessionStorage.cpp
	storage/SessionStorageArea.cpp
        storage/SQLResultSet.cpp
        storage/SQLResultSetRowList.cpp
        storage/SQLStatement.cpp
        storage/SQLTransaction.cpp
        storage/Storage.cpp
        storage/StorageArea.cpp
        storage/StorageEvent.cpp
        storage/StorageMap.cpp
    )
ENDIF (WEBKIT_USE_DATABASE)
