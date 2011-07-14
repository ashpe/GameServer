create table auth_login (
        id              integer,
        username        varchar(20),
        password        varchar(20),
        logged_in       char(80)
);

insert into auth_login (id, username, password, logged_in) values
        (1, 'ashpe', 'pass', '');

insert into auth_login (id, username, password, logged_in) values
        (2, 'ash', 'pass', '');

insert into auth_login (id, username, password, logged_in) values
        (3, 'root', '', '');
