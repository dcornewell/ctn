/* CreateControlTables.script 1.0 8-Mar-94 */
/* set nocount on */

print " "
go
print " "
go

use CTNControl
go

	create table ApplicationEntity
	(	Title	char(16) not null,
	Node	char(64) not null,
	Port		int null,
	Comment char(80) null,
	Organization char(32) not null
	)
go
	create unique index AE_Title_index
	on ApplicationEntity(Title)
go
	create index AE_Node_index
	on ApplicationEntity(Node)
go
print "Created ApplicationEntity Table"
go

	create table GroupNames
	(
	GroupName	char(16) not null,
	Title		char(16) not null
	)
go
	create index GN_GroupName_index
	on GroupNames(GroupName)
go
	create index GN_Title_index
	on GroupNames(Title)
go
print "Created GroupNames Table"
go

	create table StorageAccess (
	Title		char(16) not null,
	DbKey		char(64) not null,
	Owner		char(16) null,
	GroupName	char(16) null,
	Access		int not null,
	Comment		char(80) null
	)

go
	create unique index SA_Title_index
	on StorageAccess(Title)
go
print "Created StorageAccess Table"
go

	create table StorageControl
	(
	RequestingTitle	char(16) not null,
	RespondingTitle	char(16) not null,
	Medium			char(32) null,
	Root			char(255) not null,
	NextDirectory		char(255) null,
	Access			char(9) null
	)
go
	create index IMS_RequestingTitle_index
	on StorageControl(RequestingTitle)
go
	create index IMS_RespondingTitle_index
	on StorageControl(RespondingTitle)
go
print "Created Storage Control Table"
go

	create table SecurityMatrix
	(
	RequestingTitle	char(16) not null,
	RespondingTitle	char(16) not null,
	)
go
	create index IMS_RequestingTitle_index
	on SecurityMatrix(RequestingTitle)
go
	create index IMS_RespondingTitle_index
	on SecurityMatrix(RespondingTitle)
go
print "Created Security Matrix Table"
go

	create table FISAccess (
	Title		char(16) not null,
	DbKey		char(64) not null,
	Owner		char(16) null,
	GroupName	char(16) null,
	Access		int null,
	Comment		char(80) null
	)

go
	create unique index FA_Title_index
	on FISAccess(Title)
go
print "Created FISAccess Table"
go

	create table PrintServerCFG
	(
	RequestingTitle	char(16) not null,
	RespondingTitle	char(16) not null,
	GQId int not null
	)
go
print "Created PrintServerCFG Table"
go

	create table VideoImageDest
	(
	RequestingTitle	char(16) not null,
	ImageType	char(6) not null,
	RespondingTitle	char(16) not null
	)
go
print "Created VideoImageDest Table"
go

print " "
go
print " "
go