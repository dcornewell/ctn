/* CreateDIMTables.script 1.0 8-Mar-94 */
/* set nocount on */
print " "
go
print " "
go


use DicomImage
go
	create table Limits
	(
	DBSize		int not null,
	DBLimit		int not null,
	PatientCount	int not null,
	StudyCount	int not null,
	ImageCount	int not null
	)
go

	create table PatientLevel
	(
	PatNam		char(64) not null,
	PatID		char(64) not null,
	PatBirDat	int not null,
	PatBirTim	real null,
	PatSex		char(16) not null,
	NumPatRelStu	int not null,
	NumPatRelSer	int not null,
	NumPatRelIma	int not null,
	InsertDate	int not null,
	InsertTime	real not null,
	Owner		char(16) null,
	GroupName	char(16) null,
	Priv		char(9) null
	)
go
	create unique index PL_PatID_index
	on PatientLevel(PatID)
go
	create index PL_PatNam_index
	on PatientLevel(PatNam)
go
print "Created PatientLevel Table"
go

	create table StudyLevel
	(
	StuDat		int not null,
	StuTim		real not null,
	AccNum		char(16) not null,
	StuID		char(16) not null,
	StuInsUID	char(64) not null,
	RefPhyNam	char(64) not null,
	StuDes		char(64) null,
	PatAge		char(4) null,
	PatSiz		char(16) null,
	PatWei		char(16) null,
	NumStuRelSer	int not null,
	NumStuRelIma	int not null,
	InsertDate	int not null,
	InsertTime	real not null,
	Owner		char(16) null,
	GroupName	char(16) null,
	Priv		char(9) null,
	PatParent	char(64) not null
	)
go
	create unique index SL_StuInsUID_index
	on StudyLevel(StuInsUID)
go
	create index SL_StuID_index
	on StudyLevel(StuID)
go
	create index SL_PatParent_index
	on StudyLevel(PatParent)
go
print "Created StudyLevel Table"
go

	create table SeriesLevel
	(
	Mod		char(16) not null,
	SerNum		char(12) not null,
	SerInsUID	char(64) not null,
	ProNam		char(64) null,
	SerDes		char(64) null,
	BodParExa	char(16) null,
	ViePos		char(16) null,
	NumSerRelIma	int not null,
	InsertDate	int not null,
	InsertTime	real not null,
	Owner		char(16) null,
	GroupName	char(16) null,
	Priv		char(9) null,
	StuParent	char(64) not null
	)
go
	create unique index SL_SerInsUID_index
	on SeriesLevel(SerInsUID)
go
	create index SL_StuParent_index
	on SeriesLevel(StuParent)
go
print "Created SeriesLevel Table"
go

	create table ImageLevel
	(
	ImaNum		char(12) not null,
	SOPInsUID	char(64) not null,
	SOPClaUID	char(64) not null,
	SamPerPix	int not null,
	PhoInt		char(16) not null,
	Row		int not null,
	Col		int not null,
	BitAll		int not null,
	BitSto		int not null,
	PixRep		int not null,
	PatOri		char(16) null,
	InsertDate	int not null,
	InsertTime	real not null,
	Owner		char(16) null,
	GroupName	char(16) null,
	Priv		char(9) null,
	SerParent	char(64) not null
	)
go
	create unique index IL_SOPInsUID_index
	on ImageLevel(SOPInsUID)
go
	create index IL_SerParent_index
	on ImageLevel(SerParent)
go
print "Created ImageLevel Table"
go

	create table InstanceTable
	(
	ImageUID                char(64) not null,
	RespondingTitle char(16) not null,
	Medium                  char(32) null,
	Path                    char(255) not null,
	Size                    int not null,
	Transfer                char(64) not null
	)
go
	create index IT_ImageUID_index
	on InstanceTable(ImageUID)
go
	create index IT_RespondingTitle_index
	on InstanceTable(RespondingTitle)
go
print "Created InstanceTable Table"
go

print " "
go
print " "
go

	create view PatientStudyView
	(
	Pat_PatNam,
	Pat_PatID,
	Pat_PatBirDat,
	Pat_PatBirTim,
	Pat_PatSex,
	Pat_NumPatRelStu,
	Pat_NumPatRelSer,
	Pat_NumPatRelIma,
	Pat_InsertDate,
	Pat_InsertTime,
	Pat_Owner,
	Pat_GroupName,
	Pat_Priv,

	Stu_StuDat,
	Stu_StuTim,
	Stu_AccNum,
	Stu_StuID,
	Stu_StuInsUID,
	Stu_RefPhyNam,
	Stu_StuDes,
	Stu_PatAge,
	Stu_PatSiz,
	Stu_PatWei,
	Stu_NumStuRelSer,
	Stu_NumStuRelIma,
	Stu_InsertDate,
	Stu_InsertTime,
	Stu_Owner,
	Stu_GroupName,
	Stu_Priv,
	Stu_PatParent

	) as select 
	PatientLevel.PatNam,
	PatientLevel.PatID,
	PatientLevel.PatBirDat,
	PatientLevel.PatBirTim,
	PatientLevel.PatSex,
	PatientLevel.NumPatRelStu,
	PatientLevel.NumPatRelSer,
	PatientLevel.NumPatRelIma,
	PatientLevel.InsertDate,
	PatientLevel.InsertTime,
	PatientLevel.Owner,
	PatientLevel.GroupName,
	PatientLevel.Priv,
	StudyLevel.StuDat,
	StudyLevel.StuTim,
	StudyLevel.AccNum,
	StudyLevel.StuID,
	StudyLevel.StuInsUID,
	StudyLevel.RefPhyNam,
	StudyLevel.StuDes,
	StudyLevel.PatAge,
	StudyLevel.PatSiz,
	StudyLevel.PatWei,
	StudyLevel.NumStuRelSer,
	StudyLevel.NumStuRelIma,
	StudyLevel.InsertDate,
	StudyLevel.InsertTime,
	StudyLevel.Owner,
	StudyLevel.GroupName,
	StudyLevel.Priv,
	StudyLevel.PatParent

	from PatientLevel,StudyLevel

	where StudyLevel.PatParent = PatientLevel.PatID
go

print "Created PatientStudyView"
go

insert into Limits
	(DBSize, DBLimit, PatientCount, StudyCount, ImageCount) VALUES
	(     0,   10000,            0,          0,          0)
go

print " "
go
print " "
go

print "Inserted values into Limits table"
go
