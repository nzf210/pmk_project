PGDMP     &                    y            yhk_2021    12.3    12.3 �    �           0    0    ENCODING    ENCODING        SET client_encoding = 'UTF8';
                      false            �           0    0 
   STDSTRINGS 
   STDSTRINGS     (   SET standard_conforming_strings = 'on';
                      false            �           0    0 
   SEARCHPATH 
   SEARCHPATH     8   SELECT pg_catalog.set_config('search_path', '', false);
                      false            �           1262    51375    yhk_2021    DATABASE     �   CREATE DATABASE yhk_2021 WITH TEMPLATE = template0 ENCODING = 'UTF8' LC_COLLATE = 'Indonesian_Indonesia.1252' LC_CTYPE = 'Indonesian_Indonesia.1252';
    DROP DATABASE yhk_2021;
                postgres    false                        2615    51376    pmk_yhk    SCHEMA        CREATE SCHEMA pmk_yhk;
    DROP SCHEMA pmk_yhk;
                postgres    false            �            1255    51377    update_add()    FUNCTION     S  CREATE FUNCTION pmk_yhk.update_add() RETURNS trigger
    LANGUAGE plpgsql
    AS $$DECLARE
  BEGIN
IF tg_op = 'INSERT' THEN
UPDATE pmk_yhk.m_kampung SET pagu_add1 = pagu_add1 - new.j_cair WHERE no = new.id_kam;
RETURN new;
END IF;
   
IF tg_op = 'UPDATE' THEN
UPDATE pmk_yhk.m_kampung SET pagu_add1 = pagu_add1 + old.j_cair WHERE no = new.id_kam;
UPDATE pmk_yhk.m_kampung SET pagu_add1 = pagu_add1 - new.j_cair WHERE no = new.id_kam;
RETURN new;
END IF;
 
IF tg_op = 'DELETE' THEN
UPDATE pmk_yhk.m_kampung SET pagu_add1 = pagu_add1 + old.j_cair WHERE no = new.id_kam;
RETURN OLD;
END IF;
END$$;
 $   DROP FUNCTION pmk_yhk.update_add();
       pmk_yhk          postgres    false    6            �            1255    51604    update_blt()    FUNCTION     R  CREATE FUNCTION pmk_yhk.update_blt() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
DECLARE
  BEGIN
IF tg_op = 'INSERT' THEN
UPDATE pmk_yhk.blt SET jml_cair = jml_cair - new.jml_cair_blt WHERE id_k = new.id_kam;
RETURN new;
END IF;
   
IF tg_op = 'UPDATE' THEN
UPDATE pmk_yhk.blt SET jml_cair = jml_cair + old.jml_cair_bl WHERE id_k = new.id_kam;
UPDATE pmk_yhk.blt SET jml_cair = jml_cair - new.jml_cair_bl WHERE id_k = new.id_kam;
RETURN new;
END IF;
 
IF tg_op = 'DELETE' THEN
UPDATE pmk_yhk.blt SET jml_cair = jml_cair + old.jml_cair_bl WHERE id_k = new.id_kam;
RETURN OLD;
END IF;
END
$$;
 $   DROP FUNCTION pmk_yhk.update_blt();
       pmk_yhk          postgres    false    6            �            1255    51618    update_covid()    FUNCTION     �  CREATE FUNCTION pmk_yhk.update_covid() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
DECLARE
BEGIN
IF tg_op = 'INSERT' THEN
UPDATE pmk_yhk.covid SET jml_cair = covid.jml_cair + new.jml_cair_covid  WHERE id_kam = new.id_kam;
RETURN new;
END IF;
   
IF tg_op = 'UPDATE' THEN
UPDATE pmk_yhk.covid SET jml_cair = covid.jml_cair - old.jml_cair_covid WHERE id_kam = new.id_kam;
UPDATE pmk_yhk.covid SET jml_cair = covid.jml_cair + new.jml_cair_covid WHERE id_kam = new.id_kam;
RETURN new;
END IF;
 
IF tg_op = 'DELETE' THEN
UPDATE pmk_yhk.covid SET jml_cair = covid.jml_cair - old.jml_cair_covid WHERE id_kam = new.id_kam;
RETURN OLD;
END IF;
END;
$$;
 &   DROP FUNCTION pmk_yhk.update_covid();
       pmk_yhk          postgres    false    6            �            1255    51378    update_dds()    FUNCTION     7  CREATE FUNCTION pmk_yhk.update_dds() RETURNS trigger
    LANGUAGE plpgsql
    AS $$DECLARE
  BEGIN
IF tg_op = 'INSERT' THEN
UPDATE pmk_yhk.m_kampung SET pagu1 = pagu1 - new.j_cair WHERE no = new.id_kam;
RETURN new;
END IF;
   
IF tg_op = 'UPDATE' THEN
UPDATE pmk_yhk.m_kampung SET pagu1 = pagu1 + old.j_cair WHERE no = new.id_kam;
UPDATE pmk_yhk.m_kampung SET pagu1 = pagu1 - new.j_cair WHERE no = new.id_kam;
RETURN new;
END IF;
 
IF tg_op = 'DELETE' THEN
UPDATE pmk_yhk.m_kampung SET pagu1 = pagu1 + old.j_cair WHERE no = new.id_kam;
RETURN OLD;
END IF;
   
END$$;
 $   DROP FUNCTION pmk_yhk.update_dds();
       pmk_yhk          postgres    false    6                        1255    51609    update_sisaPagu_blt()    FUNCTION     �  CREATE FUNCTION pmk_yhk."update_sisaPagu_blt"() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
BEGIN
IF tg_op = 'INSERT' THEN
UPDATE pmk_yhk.blt SET sisa_pagu = blt.pagu - blt.jml_cair WHERE id_k = new.id_kam;
RETURN new;
END IF;
   
IF tg_op = 'UPDATE' THEN
UPDATE pmk_yhk.blt SET sisa_pagu = blt.pagu - blt.jml_cair WHERE id_k = new.id_kam;
RETURN new;
END IF;
 
IF tg_op = 'DELETE' THEN
UPDATE pmk_yhk.blt SET sisa_pagu = blt.pagu - blt.jml_cair WHERE id_k = new.id_kam;
RETURN OLD;
END IF;
END;
$$;
 /   DROP FUNCTION pmk_yhk."update_sisaPagu_blt"();
       pmk_yhk          postgres    false    6            �            1255    51613    update_sisaPagu_covid()    FUNCTION       CREATE FUNCTION pmk_yhk."update_sisaPagu_covid"() RETURNS trigger
    LANGUAGE plpgsql
    AS $$BEGIN
IF tg_op = 'INSERT' THEN
UPDATE pmk_yhk.covid SET sisa_pagu = covid.pagu - covid.jml_cair WHERE id_kam = new.id_kam;
RETURN new;
END IF;
   
IF tg_op = 'UPDATE' THEN
UPDATE pmk_yhk.covid SET sisa_pagu = covid.pagu - covid.jml_cair WHERE id_kam = new.id_kam;
RETURN new;
END IF;
 
IF tg_op = 'DELETE' THEN
UPDATE pmk_yhk.covid SET sisa_pagu = covid.pagu - covid.jml_cair WHERE id_kam = new.id_kam;
RETURN OLD;
END IF;
END;$$;
 1   DROP FUNCTION pmk_yhk."update_sisaPagu_covid"();
       pmk_yhk          postgres    false    6            �            1259    51571    blt    TABLE     �   CREATE TABLE pmk_yhk.blt (
    id_k integer NOT NULL,
    id_dis integer,
    nama_kampung character varying(255),
    nama_distrik character varying(255),
    pagu bigint,
    jml_cair bigint,
    sisa_pagu bigint,
    jml_pencairan smallint
);
    DROP TABLE pmk_yhk.blt;
       pmk_yhk         heap    postgres    false    6            �            1259    51589    blt_cair    TABLE     |   CREATE TABLE pmk_yhk.blt_cair (
    id integer NOT NULL,
    id_kam integer,
    id_dis integer,
    jml_cair_blt bigint
);
    DROP TABLE pmk_yhk.blt_cair;
       pmk_yhk         heap    postgres    false    6            �            1259    51587    blt_cair_id_seq    SEQUENCE     �   CREATE SEQUENCE pmk_yhk.blt_cair_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 '   DROP SEQUENCE pmk_yhk.blt_cair_id_seq;
       pmk_yhk          postgres    false    236    6            �           0    0    blt_cair_id_seq    SEQUENCE OWNED BY     E   ALTER SEQUENCE pmk_yhk.blt_cair_id_seq OWNED BY pmk_yhk.blt_cair.id;
          pmk_yhk          postgres    false    235            �            1259    51579    covid    TABLE       CREATE TABLE pmk_yhk.covid (
    id_kam integer NOT NULL,
    id_dis integer,
    nama_kampung character varying(255),
    nama_distrik character varying(255),
    pagu bigint,
    jml_cair bigint,
    sisa_pagu bigint,
    jml_pencairan smallint,
    pagu_pokok bigint
);
    DROP TABLE pmk_yhk.covid;
       pmk_yhk         heap    postgres    false    6            �            1259    51595 
   covid_cair    TABLE     �   CREATE TABLE pmk_yhk.covid_cair (
    id integer NOT NULL,
    id_kam integer,
    id_dis integer,
    jml_cair_covid bigint
);
    DROP TABLE pmk_yhk.covid_cair;
       pmk_yhk         heap    postgres    false    6            �            1259    51615    covid_cair_seq    SEQUENCE     x   CREATE SEQUENCE pmk_yhk.covid_cair_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 &   DROP SEQUENCE pmk_yhk.covid_cair_seq;
       pmk_yhk          postgres    false    237    6            �           0    0    covid_cair_seq    SEQUENCE OWNED BY     F   ALTER SEQUENCE pmk_yhk.covid_cair_seq OWNED BY pmk_yhk.covid_cair.id;
          pmk_yhk          postgres    false    238            �            1259    51561    dds_2021    TABLE     �  CREATE TABLE pmk_yhk.dds_2021 (
    id integer NOT NULL,
    nama_distrik character varying(255),
    nama_kampung character varying(255),
    pagu_dana_desa bigint,
    blt bigint,
    covid bigint,
    r_tahap_i bigint,
    r_tahap_ii bigint,
    r_tahap_3 bigint,
    pagu_reguler bigint,
    sisa_pagu_reguler bigint,
    blt_t1 bigint,
    blt_t2 bigint,
    blt_t3 bigint,
    blt_t4 bigint,
    blt_t5 bigint,
    blt_t6 bigint,
    blt_t7 bigint,
    blt_t8 bigint,
    blt_t9 bigint,
    blt_t10 bigint,
    blt_t11 bigint,
    blt_t12 bigint,
    cair_tr1 bigint,
    cair_tr2 bigint,
    cair_tr3 bigint,
    cvd_1 bigint,
    cvd_2 bigint,
    cvd_3 bigint,
    sisa_blt bigint,
    sisa_covid bigint,
    cvd_4 bigint
);
    DROP TABLE pmk_yhk.dds_2021;
       pmk_yhk         heap    postgres    false    6            �            1259    51559    dds_2021_id_seq    SEQUENCE     �   CREATE SEQUENCE pmk_yhk.dds_2021_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 '   DROP SEQUENCE pmk_yhk.dds_2021_id_seq;
       pmk_yhk          postgres    false    6    232            �           0    0    dds_2021_id_seq    SEQUENCE OWNED BY     E   ALTER SEQUENCE pmk_yhk.dds_2021_id_seq OWNED BY pmk_yhk.dds_2021.id;
          pmk_yhk          postgres    false    231            �            1259    51379    m_bam    TABLE     w  CREATE TABLE pmk_yhk.m_bam (
    id integer NOT NULL,
    id_kam integer,
    distrik character varying(255),
    kampung character varying(255),
    jabatan character varying(255),
    no_sk character varying(255),
    tgl_sk date,
    ttl character varying(255),
    alamat character varying(255),
    nama character varying(255),
    id_j smallint,
    id_dis smallint
);
    DROP TABLE pmk_yhk.m_bam;
       pmk_yhk         heap    postgres    false    6            �            1259    51385 	   m_jabatan    TABLE     c   CREATE TABLE pmk_yhk.m_jabatan (
    id_j smallint NOT NULL,
    jabatan character varying(255)
);
    DROP TABLE pmk_yhk.m_jabatan;
       pmk_yhk         heap    postgres    false    6            �            1259    51388    m_jabatan_id_j_seq    SEQUENCE     �   CREATE SEQUENCE pmk_yhk.m_jabatan_id_j_seq
    AS smallint
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 *   DROP SEQUENCE pmk_yhk.m_jabatan_id_j_seq;
       pmk_yhk          postgres    false    204    6            �           0    0    m_jabatan_id_j_seq    SEQUENCE OWNED BY     K   ALTER SEQUENCE pmk_yhk.m_jabatan_id_j_seq OWNED BY pmk_yhk.m_jabatan.id_j;
          pmk_yhk          postgres    false    205            �            1259    51390 	   m_kampung    TABLE     $  CREATE TABLE pmk_yhk.m_kampung (
    no integer NOT NULL,
    distrik character varying(255),
    kampung character varying(255),
    pagu double precision,
    pagu1 double precision,
    id_distrik integer,
    pagu_add double precision,
    pagu_add1 double precision,
    j_kk integer
);
    DROP TABLE pmk_yhk.m_kampung;
       pmk_yhk         heap    postgres    false    6            �            1259    51396    m_rek    TABLE     9  CREATE TABLE pmk_yhk.m_rek (
    id integer NOT NULL,
    no_rek character varying(255),
    kas_kampung character varying(255),
    dana_kampung character varying(255),
    nm_bank character varying(255),
    j_kk character varying(255),
    men_pmk character varying(255),
    men_keu character varying(255)
);
    DROP TABLE pmk_yhk.m_rek;
       pmk_yhk         heap    postgres    false    6            �            1259    51402    nm_bam    TABLE       CREATE TABLE pmk_yhk.nm_bam (
    id bigint NOT NULL,
    nama_1 character varying(255),
    nama_2 character varying(255),
    nama_3 character varying(255),
    nama_4 character varying(255),
    nama_5 character varying(255),
    nama_6 character varying(255),
    nama_7 character varying(255),
    nama_8 character varying(255),
    nama_9 character varying(255),
    nama_10 character varying(255),
    nama_11 character varying(255),
    nama_12 character varying(255),
    nama_13 character varying(255)
);
    DROP TABLE pmk_yhk.nm_bam;
       pmk_yhk         heap    postgres    false    6            �            1259    51408    nm_bam_id_seq    SEQUENCE     w   CREATE SEQUENCE pmk_yhk.nm_bam_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 %   DROP SEQUENCE pmk_yhk.nm_bam_id_seq;
       pmk_yhk          postgres    false    6    208            �           0    0    nm_bam_id_seq    SEQUENCE OWNED BY     A   ALTER SEQUENCE pmk_yhk.nm_bam_id_seq OWNED BY pmk_yhk.nm_bam.id;
          pmk_yhk          postgres    false    209            �            1259    51410    sppdd    TABLE     �   CREATE TABLE pmk_yhk.sppdd (
    id integer NOT NULL,
    no_srt character varying(255),
    perihal character varying(255),
    tgl date,
    nos integer
);
    DROP TABLE pmk_yhk.sppdd;
       pmk_yhk         heap    postgres    false    6            �            1259    51416    sppdd_id_seq    SEQUENCE     �   CREATE SEQUENCE pmk_yhk.sppdd_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 $   DROP SEQUENCE pmk_yhk.sppdd_id_seq;
       pmk_yhk          postgres    false    210    6            �           0    0    sppdd_id_seq    SEQUENCE OWNED BY     ?   ALTER SEQUENCE pmk_yhk.sppdd_id_seq OWNED BY pmk_yhk.sppdd.id;
          pmk_yhk          postgres    false    211            �            1259    51418    sppdd_2    TABLE     �   CREATE TABLE pmk_yhk.sppdd_2 (
    id integer DEFAULT nextval('pmk_yhk.sppdd_id_seq'::regclass) NOT NULL,
    no_srt character varying(255),
    perihal character varying(255),
    tgl date,
    nos integer
);
    DROP TABLE pmk_yhk.sppdd_2;
       pmk_yhk         heap    postgres    false    211    6            �            1259    51425    t_cair    TABLE       CREATE TABLE pmk_yhk.t_cair (
    id_cair bigint NOT NULL,
    id_kampung integer NOT NULL,
    "jumlah cair" double precision,
    nm_kepala character varying(255),
    nm_ben character varying(255),
    no_pengantar character varying(255),
    no_perubahan character varying(255)
);
    DROP TABLE pmk_yhk.t_cair;
       pmk_yhk         heap    postgres    false    6            �            1259    51431    t_cair_id_cair_seq    SEQUENCE     |   CREATE SEQUENCE pmk_yhk.t_cair_id_cair_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 *   DROP SEQUENCE pmk_yhk.t_cair_id_cair_seq;
       pmk_yhk          postgres    false    213    6            �           0    0    t_cair_id_cair_seq    SEQUENCE OWNED BY     K   ALTER SEQUENCE pmk_yhk.t_cair_id_cair_seq OWNED BY pmk_yhk.t_cair.id_cair;
          pmk_yhk          postgres    false    214            �            1259    51433    t_real    TABLE     �  CREATE TABLE pmk_yhk.t_real (
    id_real bigint NOT NULL,
    id_kam integer,
    nm_dis character varying(255),
    nm_kam character varying(255),
    no_rek character varying(255),
    nm_rek character varying(255),
    nm_bank character varying(255),
    sk_bend character varying(255),
    sk_kp character varying(255),
    nm_ben character varying(255),
    nm_kp character varying(255),
    thp_cair character varying(255),
    j_cair double precision,
    j_terbilang text,
    tgl date,
    no_srt1 character varying(255),
    no_srt2 character varying(255),
    persentase character varying(255),
    sk_bup character varying(255),
    sk_kam character varying(255),
    sk_pmk character varying(255),
    sk_keu character varying(255),
    nm_kpd character varying(255),
    j_kpd character varying(255),
    nip_kpd character varying(255),
    ket text,
    thp_l character varying(255),
    id bigint NOT NULL,
    jkk integer,
    nos integer
);
    DROP TABLE pmk_yhk.t_real;
       pmk_yhk         heap    postgres    false    6            �            1259    51439    t_real_id_real_seq    SEQUENCE     |   CREATE SEQUENCE pmk_yhk.t_real_id_real_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 *   DROP SEQUENCE pmk_yhk.t_real_id_real_seq;
       pmk_yhk          postgres    false    215    6            �           0    0    t_real_id_real_seq    SEQUENCE OWNED BY     K   ALTER SEQUENCE pmk_yhk.t_real_id_real_seq OWNED BY pmk_yhk.t_real.id_real;
          pmk_yhk          postgres    false    216            �            1259    51441    t_real_2    TABLE     �  CREATE TABLE pmk_yhk.t_real_2 (
    id_real bigint DEFAULT nextval('pmk_yhk.t_real_id_real_seq'::regclass) NOT NULL,
    id_kam integer,
    nm_dis character varying(255),
    nm_kam character varying(255),
    no_rek character varying(255),
    nm_rek character varying(255),
    nm_bank character varying(255),
    sk_bend character varying(255),
    sk_kp character varying(255),
    nm_ben character varying(255),
    nm_kp character varying(255),
    thp_cair character varying(255),
    j_cair double precision,
    j_terbilang text,
    tgl date,
    no_srt1 character varying(255),
    no_srt2 character varying(255),
    persentase character varying(255),
    sk_bup character varying(255),
    sk_kam character varying(255),
    sk_pmk character varying(255),
    sk_keu character varying(255),
    nm_kpd character varying(255),
    j_kpd character varying(255),
    nip_kpd character varying(255),
    ket text,
    thp_l character varying(255),
    id bigint NOT NULL,
    jkk integer,
    nos integer
);
    DROP TABLE pmk_yhk.t_real_2;
       pmk_yhk         heap    postgres    false    216    6            �            1259    51448    t_real_2_id_seq    SEQUENCE     y   CREATE SEQUENCE pmk_yhk.t_real_2_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 '   DROP SEQUENCE pmk_yhk.t_real_2_id_seq;
       pmk_yhk          postgres    false    6    217            �           0    0    t_real_2_id_seq    SEQUENCE OWNED BY     E   ALTER SEQUENCE pmk_yhk.t_real_2_id_seq OWNED BY pmk_yhk.t_real_2.id;
          pmk_yhk          postgres    false    218            �            1259    51450    t_real_id_seq    SEQUENCE     w   CREATE SEQUENCE pmk_yhk.t_real_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 %   DROP SEQUENCE pmk_yhk.t_real_id_seq;
       pmk_yhk          postgres    false    6    215            �           0    0    t_real_id_seq    SEQUENCE OWNED BY     A   ALTER SEQUENCE pmk_yhk.t_real_id_seq OWNED BY pmk_yhk.t_real.id;
          pmk_yhk          postgres    false    219            �            1259    51452 	   tahap_add    TABLE     �   CREATE TABLE pmk_yhk.tahap_add (
    id integer NOT NULL,
    tahap character varying(255),
    persen character varying(255),
    laporan character varying(255),
    thp character varying(255)
);
    DROP TABLE pmk_yhk.tahap_add;
       pmk_yhk         heap    postgres    false    6            �            1259    51458 	   tahap_dds    TABLE     �   CREATE TABLE pmk_yhk.tahap_dds (
    id integer NOT NULL,
    tahap character varying(255),
    persen character varying(255),
    laporan character varying(255),
    thp character varying(255)
);
    DROP TABLE pmk_yhk.tahap_dds;
       pmk_yhk         heap    postgres    false    6            �            1259    51464    usr    TABLE     
  CREATE TABLE pmk_yhk.usr (
    id integer NOT NULL,
    nama character varying(255),
    jabatan character varying(255),
    pass character varying(255),
    level character varying(255),
    type character varying(255),
    id_lvl smallint,
    id_type smallint
);
    DROP TABLE pmk_yhk.usr;
       pmk_yhk         heap    postgres    false    6            �            1259    51470 
   usr_id_seq    SEQUENCE     �   CREATE SEQUENCE pmk_yhk.usr_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 "   DROP SEQUENCE pmk_yhk.usr_id_seq;
       pmk_yhk          postgres    false    222    6            �           0    0 
   usr_id_seq    SEQUENCE OWNED BY     ;   ALTER SEQUENCE pmk_yhk.usr_id_seq OWNED BY pmk_yhk.usr.id;
          pmk_yhk          postgres    false    223            �            1259    51472    v_bam    VIEW     e  CREATE VIEW pmk_yhk.v_bam AS
 SELECT m_kampung.id_distrik,
    m_kampung.no,
    m_kampung.distrik,
    m_kampung.kampung,
    m_kampung.pagu,
    m_kampung.pagu1,
    (m_kampung.pagu - m_kampung.pagu1) AS realisasi,
    m_kampung.pagu_add,
    m_kampung.pagu_add1,
    (m_kampung.pagu_add - m_kampung.pagu_add1) AS realisasi_add
   FROM pmk_yhk.m_kampung;
    DROP VIEW pmk_yhk.v_bam;
       pmk_yhk          postgres    false    206    206    206    206    206    206    206    206    6            �            1259    51476    v_bam_    VIEW     �   CREATE VIEW pmk_yhk.v_bam_ AS
 SELECT a.id,
    a.id_kam,
    a.id_j AS id_dis,
    a.id_dis AS id_j,
    a.distrik,
    a.kampung,
    a.nama,
    a.jabatan,
    a.no_sk,
    a.tgl_sk,
    a.ttl,
    a.alamat
   FROM pmk_yhk.m_bam a;
    DROP VIEW pmk_yhk.v_bam_;
       pmk_yhk          postgres    false    203    203    203    203    203    203    203    203    203    203    203    203    6            �            1259    51480 	   v_bam_add    VIEW       CREATE VIEW pmk_yhk.v_bam_add AS
 SELECT m_kampung.id_distrik,
    m_kampung.no,
    m_kampung.distrik,
    m_kampung.kampung,
    m_kampung.pagu_add,
    m_kampung.pagu_add1,
    (m_kampung.pagu_add - m_kampung.pagu_add1) AS realisasi
   FROM pmk_yhk.m_kampung;
    DROP VIEW pmk_yhk.v_bam_add;
       pmk_yhk          postgres    false    206    206    206    206    206    206    6            �            1259    51484    v_dis    VIEW     �   CREATE VIEW pmk_yhk.v_dis AS
 SELECT a.distrik,
    a.id_distrik
   FROM pmk_yhk.m_kampung a
  GROUP BY a.distrik, a.id_distrik;
    DROP VIEW pmk_yhk.v_dis;
       pmk_yhk          postgres    false    206    206    6            �            1259    51488    v_kam    VIEW     V   CREATE VIEW pmk_yhk.v_kam AS
 SELECT a.kampung,
    a.no
   FROM pmk_yhk.m_kampung a;
    DROP VIEW pmk_yhk.v_kam;
       pmk_yhk          postgres    false    206    206    6            �            1259    51492    ver    TABLE     �   CREATE TABLE pmk_yhk.ver (
    id smallint NOT NULL,
    ver character varying(16),
    date date,
    ket character varying(255),
    enc character varying(255)
);
    DROP TABLE pmk_yhk.ver;
       pmk_yhk         heap    postgres    false    6            �            1259    51498 
   ver_id_seq    SEQUENCE     �   CREATE SEQUENCE pmk_yhk.ver_id_seq
    AS smallint
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 "   DROP SEQUENCE pmk_yhk.ver_id_seq;
       pmk_yhk          postgres    false    229    6            �           0    0 
   ver_id_seq    SEQUENCE OWNED BY     ;   ALTER SEQUENCE pmk_yhk.ver_id_seq OWNED BY pmk_yhk.ver.id;
          pmk_yhk          postgres    false    230                       2604    51592    blt_cair id    DEFAULT     l   ALTER TABLE ONLY pmk_yhk.blt_cair ALTER COLUMN id SET DEFAULT nextval('pmk_yhk.blt_cair_id_seq'::regclass);
 ;   ALTER TABLE pmk_yhk.blt_cair ALTER COLUMN id DROP DEFAULT;
       pmk_yhk          postgres    false    235    236    236                       2604    51617    covid_cair id    DEFAULT     m   ALTER TABLE ONLY pmk_yhk.covid_cair ALTER COLUMN id SET DEFAULT nextval('pmk_yhk.covid_cair_seq'::regclass);
 =   ALTER TABLE pmk_yhk.covid_cair ALTER COLUMN id DROP DEFAULT;
       pmk_yhk          postgres    false    238    237                       2604    51564    dds_2021 id    DEFAULT     l   ALTER TABLE ONLY pmk_yhk.dds_2021 ALTER COLUMN id SET DEFAULT nextval('pmk_yhk.dds_2021_id_seq'::regclass);
 ;   ALTER TABLE pmk_yhk.dds_2021 ALTER COLUMN id DROP DEFAULT;
       pmk_yhk          postgres    false    231    232    232                       2604    51500    m_jabatan id_j    DEFAULT     r   ALTER TABLE ONLY pmk_yhk.m_jabatan ALTER COLUMN id_j SET DEFAULT nextval('pmk_yhk.m_jabatan_id_j_seq'::regclass);
 >   ALTER TABLE pmk_yhk.m_jabatan ALTER COLUMN id_j DROP DEFAULT;
       pmk_yhk          postgres    false    205    204            	           2604    51501 	   nm_bam id    DEFAULT     h   ALTER TABLE ONLY pmk_yhk.nm_bam ALTER COLUMN id SET DEFAULT nextval('pmk_yhk.nm_bam_id_seq'::regclass);
 9   ALTER TABLE pmk_yhk.nm_bam ALTER COLUMN id DROP DEFAULT;
       pmk_yhk          postgres    false    209    208            
           2604    51502    sppdd id    DEFAULT     f   ALTER TABLE ONLY pmk_yhk.sppdd ALTER COLUMN id SET DEFAULT nextval('pmk_yhk.sppdd_id_seq'::regclass);
 8   ALTER TABLE pmk_yhk.sppdd ALTER COLUMN id DROP DEFAULT;
       pmk_yhk          postgres    false    211    210                       2604    51503    t_cair id_cair    DEFAULT     r   ALTER TABLE ONLY pmk_yhk.t_cair ALTER COLUMN id_cair SET DEFAULT nextval('pmk_yhk.t_cair_id_cair_seq'::regclass);
 >   ALTER TABLE pmk_yhk.t_cair ALTER COLUMN id_cair DROP DEFAULT;
       pmk_yhk          postgres    false    214    213                       2604    51504    t_real id_real    DEFAULT     r   ALTER TABLE ONLY pmk_yhk.t_real ALTER COLUMN id_real SET DEFAULT nextval('pmk_yhk.t_real_id_real_seq'::regclass);
 >   ALTER TABLE pmk_yhk.t_real ALTER COLUMN id_real DROP DEFAULT;
       pmk_yhk          postgres    false    216    215                       2604    51505 	   t_real id    DEFAULT     h   ALTER TABLE ONLY pmk_yhk.t_real ALTER COLUMN id SET DEFAULT nextval('pmk_yhk.t_real_id_seq'::regclass);
 9   ALTER TABLE pmk_yhk.t_real ALTER COLUMN id DROP DEFAULT;
       pmk_yhk          postgres    false    219    215                       2604    51506    t_real_2 id    DEFAULT     l   ALTER TABLE ONLY pmk_yhk.t_real_2 ALTER COLUMN id SET DEFAULT nextval('pmk_yhk.t_real_2_id_seq'::regclass);
 ;   ALTER TABLE pmk_yhk.t_real_2 ALTER COLUMN id DROP DEFAULT;
       pmk_yhk          postgres    false    218    217                       2604    51507    usr id    DEFAULT     b   ALTER TABLE ONLY pmk_yhk.usr ALTER COLUMN id SET DEFAULT nextval('pmk_yhk.usr_id_seq'::regclass);
 6   ALTER TABLE pmk_yhk.usr ALTER COLUMN id DROP DEFAULT;
       pmk_yhk          postgres    false    223    222                       2604    51508    ver id    DEFAULT     b   ALTER TABLE ONLY pmk_yhk.ver ALTER COLUMN id SET DEFAULT nextval('pmk_yhk.ver_id_seq'::regclass);
 6   ALTER TABLE pmk_yhk.ver ALTER COLUMN id DROP DEFAULT;
       pmk_yhk          postgres    false    230    229            �          0    51571    blt 
   TABLE DATA           r   COPY pmk_yhk.blt (id_k, id_dis, nama_kampung, nama_distrik, pagu, jml_cair, sisa_pagu, jml_pencairan) FROM stdin;
    pmk_yhk          postgres    false    233   �       �          0    51589    blt_cair 
   TABLE DATA           E   COPY pmk_yhk.blt_cair (id, id_kam, id_dis, jml_cair_blt) FROM stdin;
    pmk_yhk          postgres    false    236   +�       �          0    51579    covid 
   TABLE DATA           �   COPY pmk_yhk.covid (id_kam, id_dis, nama_kampung, nama_distrik, pagu, jml_cair, sisa_pagu, jml_pencairan, pagu_pokok) FROM stdin;
    pmk_yhk          postgres    false    234   �       �          0    51595 
   covid_cair 
   TABLE DATA           I   COPY pmk_yhk.covid_cair (id, id_kam, id_dis, jml_cair_covid) FROM stdin;
    pmk_yhk          postgres    false    237   �       �          0    51561    dds_2021 
   TABLE DATA           U  COPY pmk_yhk.dds_2021 (id, nama_distrik, nama_kampung, pagu_dana_desa, blt, covid, r_tahap_i, r_tahap_ii, r_tahap_3, pagu_reguler, sisa_pagu_reguler, blt_t1, blt_t2, blt_t3, blt_t4, blt_t5, blt_t6, blt_t7, blt_t8, blt_t9, blt_t10, blt_t11, blt_t12, cair_tr1, cair_tr2, cair_tr3, cvd_1, cvd_2, cvd_3, sisa_blt, sisa_covid, cvd_4) FROM stdin;
    pmk_yhk          postgres    false    232   ��       �          0    51379    m_bam 
   TABLE DATA           w   COPY pmk_yhk.m_bam (id, id_kam, distrik, kampung, jabatan, no_sk, tgl_sk, ttl, alamat, nama, id_j, id_dis) FROM stdin;
    pmk_yhk          postgres    false    203   �      �          0    51385 	   m_jabatan 
   TABLE DATA           3   COPY pmk_yhk.m_jabatan (id_j, jabatan) FROM stdin;
    pmk_yhk          postgres    false    204   �w      �          0    51390 	   m_kampung 
   TABLE DATA           n   COPY pmk_yhk.m_kampung (no, distrik, kampung, pagu, pagu1, id_distrik, pagu_add, pagu_add1, j_kk) FROM stdin;
    pmk_yhk          postgres    false    206   �x      �          0    51396    m_rek 
   TABLE DATA           h   COPY pmk_yhk.m_rek (id, no_rek, kas_kampung, dana_kampung, nm_bank, j_kk, men_pmk, men_keu) FROM stdin;
    pmk_yhk          postgres    false    207   ��      �          0    51402    nm_bam 
   TABLE DATA           �   COPY pmk_yhk.nm_bam (id, nama_1, nama_2, nama_3, nama_4, nama_5, nama_6, nama_7, nama_8, nama_9, nama_10, nama_11, nama_12, nama_13) FROM stdin;
    pmk_yhk          postgres    false    208   Z�      �          0    51410    sppdd 
   TABLE DATA           ?   COPY pmk_yhk.sppdd (id, no_srt, perihal, tgl, nos) FROM stdin;
    pmk_yhk          postgres    false    210   w�      �          0    51418    sppdd_2 
   TABLE DATA           A   COPY pmk_yhk.sppdd_2 (id, no_srt, perihal, tgl, nos) FROM stdin;
    pmk_yhk          postgres    false    212   ��      �          0    51425    t_cair 
   TABLE DATA           t   COPY pmk_yhk.t_cair (id_cair, id_kampung, "jumlah cair", nm_kepala, nm_ben, no_pengantar, no_perubahan) FROM stdin;
    pmk_yhk          postgres    false    213   ��      �          0    51433    t_real 
   TABLE DATA             COPY pmk_yhk.t_real (id_real, id_kam, nm_dis, nm_kam, no_rek, nm_rek, nm_bank, sk_bend, sk_kp, nm_ben, nm_kp, thp_cair, j_cair, j_terbilang, tgl, no_srt1, no_srt2, persentase, sk_bup, sk_kam, sk_pmk, sk_keu, nm_kpd, j_kpd, nip_kpd, ket, thp_l, id, jkk, nos) FROM stdin;
    pmk_yhk          postgres    false    215   ��      �          0    51441    t_real_2 
   TABLE DATA             COPY pmk_yhk.t_real_2 (id_real, id_kam, nm_dis, nm_kam, no_rek, nm_rek, nm_bank, sk_bend, sk_kp, nm_ben, nm_kp, thp_cair, j_cair, j_terbilang, tgl, no_srt1, no_srt2, persentase, sk_bup, sk_kam, sk_pmk, sk_keu, nm_kpd, j_kpd, nip_kpd, ket, thp_l, id, jkk, nos) FROM stdin;
    pmk_yhk          postgres    false    217   ��      �          0    51452 	   tahap_add 
   TABLE DATA           E   COPY pmk_yhk.tahap_add (id, tahap, persen, laporan, thp) FROM stdin;
    pmk_yhk          postgres    false    220   �      �          0    51458 	   tahap_dds 
   TABLE DATA           E   COPY pmk_yhk.tahap_dds (id, tahap, persen, laporan, thp) FROM stdin;
    pmk_yhk          postgres    false    221   ��      �          0    51464    usr 
   TABLE DATA           U   COPY pmk_yhk.usr (id, nama, jabatan, pass, level, type, id_lvl, id_type) FROM stdin;
    pmk_yhk          postgres    false    222   n�      �          0    51492    ver 
   TABLE DATA           7   COPY pmk_yhk.ver (id, ver, date, ket, enc) FROM stdin;
    pmk_yhk          postgres    false    229   ��      �           0    0    blt_cair_id_seq    SEQUENCE SET     @   SELECT pg_catalog.setval('pmk_yhk.blt_cair_id_seq', 297, true);
          pmk_yhk          postgres    false    235            �           0    0    covid_cair_seq    SEQUENCE SET     >   SELECT pg_catalog.setval('pmk_yhk.covid_cair_seq', 48, true);
          pmk_yhk          postgres    false    238            �           0    0    dds_2021_id_seq    SEQUENCE SET     ?   SELECT pg_catalog.setval('pmk_yhk.dds_2021_id_seq', 1, false);
          pmk_yhk          postgres    false    231            �           0    0    m_jabatan_id_j_seq    SEQUENCE SET     B   SELECT pg_catalog.setval('pmk_yhk.m_jabatan_id_j_seq', 1, false);
          pmk_yhk          postgres    false    205            �           0    0    nm_bam_id_seq    SEQUENCE SET     =   SELECT pg_catalog.setval('pmk_yhk.nm_bam_id_seq', 1, false);
          pmk_yhk          postgres    false    209            �           0    0    sppdd_id_seq    SEQUENCE SET     <   SELECT pg_catalog.setval('pmk_yhk.sppdd_id_seq', 89, true);
          pmk_yhk          postgres    false    211            �           0    0    t_cair_id_cair_seq    SEQUENCE SET     B   SELECT pg_catalog.setval('pmk_yhk.t_cair_id_cair_seq', 1, false);
          pmk_yhk          postgres    false    214            �           0    0    t_real_2_id_seq    SEQUENCE SET     ?   SELECT pg_catalog.setval('pmk_yhk.t_real_2_id_seq', 91, true);
          pmk_yhk          postgres    false    218                        0    0    t_real_id_real_seq    SEQUENCE SET     C   SELECT pg_catalog.setval('pmk_yhk.t_real_id_real_seq', 768, true);
          pmk_yhk          postgres    false    216                       0    0    t_real_id_seq    SEQUENCE SET     >   SELECT pg_catalog.setval('pmk_yhk.t_real_id_seq', 658, true);
          pmk_yhk          postgres    false    219                       0    0 
   usr_id_seq    SEQUENCE SET     :   SELECT pg_catalog.setval('pmk_yhk.usr_id_seq', 18, true);
          pmk_yhk          postgres    false    223                       0    0 
   ver_id_seq    SEQUENCE SET     9   SELECT pg_catalog.setval('pmk_yhk.ver_id_seq', 1, true);
          pmk_yhk          postgres    false    230            ;           2606    51600    covid_cair blt_cair_copy1_pkey 
   CONSTRAINT     ]   ALTER TABLE ONLY pmk_yhk.covid_cair
    ADD CONSTRAINT blt_cair_copy1_pkey PRIMARY KEY (id);
 I   ALTER TABLE ONLY pmk_yhk.covid_cair DROP CONSTRAINT blt_cair_copy1_pkey;
       pmk_yhk            postgres    false    237            9           2606    51594    blt_cair blt_cair_pkey 
   CONSTRAINT     U   ALTER TABLE ONLY pmk_yhk.blt_cair
    ADD CONSTRAINT blt_cair_pkey PRIMARY KEY (id);
 A   ALTER TABLE ONLY pmk_yhk.blt_cair DROP CONSTRAINT blt_cair_pkey;
       pmk_yhk            postgres    false    236            7           2606    51586    covid blt_copy1_pkey 
   CONSTRAINT     W   ALTER TABLE ONLY pmk_yhk.covid
    ADD CONSTRAINT blt_copy1_pkey PRIMARY KEY (id_kam);
 ?   ALTER TABLE ONLY pmk_yhk.covid DROP CONSTRAINT blt_copy1_pkey;
       pmk_yhk            postgres    false    234            5           2606    51578    blt blt_pkey 
   CONSTRAINT     M   ALTER TABLE ONLY pmk_yhk.blt
    ADD CONSTRAINT blt_pkey PRIMARY KEY (id_k);
 7   ALTER TABLE ONLY pmk_yhk.blt DROP CONSTRAINT blt_pkey;
       pmk_yhk            postgres    false    233            3           2606    51569    dds_2021 dds_2021_pkey 
   CONSTRAINT     U   ALTER TABLE ONLY pmk_yhk.dds_2021
    ADD CONSTRAINT dds_2021_pkey PRIMARY KEY (id);
 A   ALTER TABLE ONLY pmk_yhk.dds_2021 DROP CONSTRAINT dds_2021_pkey;
       pmk_yhk            postgres    false    232                       2606    51510    m_bam m_bam_pkey 
   CONSTRAINT     O   ALTER TABLE ONLY pmk_yhk.m_bam
    ADD CONSTRAINT m_bam_pkey PRIMARY KEY (id);
 ;   ALTER TABLE ONLY pmk_yhk.m_bam DROP CONSTRAINT m_bam_pkey;
       pmk_yhk            postgres    false    203                       2606    51512    m_rek m_rek_pkey 
   CONSTRAINT     O   ALTER TABLE ONLY pmk_yhk.m_rek
    ADD CONSTRAINT m_rek_pkey PRIMARY KEY (id);
 ;   ALTER TABLE ONLY pmk_yhk.m_rek DROP CONSTRAINT m_rek_pkey;
       pmk_yhk            postgres    false    207                       2606    51514    m_kampung master_kampung_pkey 
   CONSTRAINT     \   ALTER TABLE ONLY pmk_yhk.m_kampung
    ADD CONSTRAINT master_kampung_pkey PRIMARY KEY (no);
 H   ALTER TABLE ONLY pmk_yhk.m_kampung DROP CONSTRAINT master_kampung_pkey;
       pmk_yhk            postgres    false    206                       2606    51516    nm_bam nm_bam_pkey 
   CONSTRAINT     Q   ALTER TABLE ONLY pmk_yhk.nm_bam
    ADD CONSTRAINT nm_bam_pkey PRIMARY KEY (id);
 =   ALTER TABLE ONLY pmk_yhk.nm_bam DROP CONSTRAINT nm_bam_pkey;
       pmk_yhk            postgres    false    208            #           2606    51518    sppdd_2 sppdd_copy1_pkey 
   CONSTRAINT     W   ALTER TABLE ONLY pmk_yhk.sppdd_2
    ADD CONSTRAINT sppdd_copy1_pkey PRIMARY KEY (id);
 C   ALTER TABLE ONLY pmk_yhk.sppdd_2 DROP CONSTRAINT sppdd_copy1_pkey;
       pmk_yhk            postgres    false    212                       2606    51520    sppdd sppdd_pkey 
   CONSTRAINT     O   ALTER TABLE ONLY pmk_yhk.sppdd
    ADD CONSTRAINT sppdd_pkey PRIMARY KEY (id);
 ;   ALTER TABLE ONLY pmk_yhk.sppdd DROP CONSTRAINT sppdd_pkey;
       pmk_yhk            postgres    false    210            '           2606    51522    t_cair t_cair_pkey 
   CONSTRAINT     V   ALTER TABLE ONLY pmk_yhk.t_cair
    ADD CONSTRAINT t_cair_pkey PRIMARY KEY (id_cair);
 =   ALTER TABLE ONLY pmk_yhk.t_cair DROP CONSTRAINT t_cair_pkey;
       pmk_yhk            postgres    false    213            +           2606    51524    t_real_2 t_real_copy1_pkey 
   CONSTRAINT     Y   ALTER TABLE ONLY pmk_yhk.t_real_2
    ADD CONSTRAINT t_real_copy1_pkey PRIMARY KEY (id);
 E   ALTER TABLE ONLY pmk_yhk.t_real_2 DROP CONSTRAINT t_real_copy1_pkey;
       pmk_yhk            postgres    false    217            )           2606    51526    t_real t_real_pkey 
   CONSTRAINT     Q   ALTER TABLE ONLY pmk_yhk.t_real
    ADD CONSTRAINT t_real_pkey PRIMARY KEY (id);
 =   ALTER TABLE ONLY pmk_yhk.t_real DROP CONSTRAINT t_real_pkey;
       pmk_yhk            postgres    false    215            -           2606    51528    tahap_add tahap_dds_copy1_pkey 
   CONSTRAINT     ]   ALTER TABLE ONLY pmk_yhk.tahap_add
    ADD CONSTRAINT tahap_dds_copy1_pkey PRIMARY KEY (id);
 I   ALTER TABLE ONLY pmk_yhk.tahap_add DROP CONSTRAINT tahap_dds_copy1_pkey;
       pmk_yhk            postgres    false    220            /           2606    51530    tahap_dds tahap_dds_pkey 
   CONSTRAINT     W   ALTER TABLE ONLY pmk_yhk.tahap_dds
    ADD CONSTRAINT tahap_dds_pkey PRIMARY KEY (id);
 C   ALTER TABLE ONLY pmk_yhk.tahap_dds DROP CONSTRAINT tahap_dds_pkey;
       pmk_yhk            postgres    false    221            !           2606    51532 	   sppdd tgl 
   CONSTRAINT     D   ALTER TABLE ONLY pmk_yhk.sppdd
    ADD CONSTRAINT tgl UNIQUE (tgl);
 4   ALTER TABLE ONLY pmk_yhk.sppdd DROP CONSTRAINT tgl;
       pmk_yhk            postgres    false    210            %           2606    51534    sppdd_2 tgl_ 
   CONSTRAINT     G   ALTER TABLE ONLY pmk_yhk.sppdd_2
    ADD CONSTRAINT tgl_ UNIQUE (tgl);
 7   ALTER TABLE ONLY pmk_yhk.sppdd_2 DROP CONSTRAINT tgl_;
       pmk_yhk            postgres    false    212            1           2606    51536    usr usr_pkey 
   CONSTRAINT     K   ALTER TABLE ONLY pmk_yhk.usr
    ADD CONSTRAINT usr_pkey PRIMARY KEY (id);
 7   ALTER TABLE ONLY pmk_yhk.usr DROP CONSTRAINT usr_pkey;
       pmk_yhk            postgres    false    222            C           2620    51537    t_real_2 update_add    TRIGGER     �   CREATE TRIGGER update_add AFTER INSERT OR DELETE OR UPDATE ON pmk_yhk.t_real_2 FOR EACH ROW EXECUTE FUNCTION pmk_yhk.update_add();
 -   DROP TRIGGER update_add ON pmk_yhk.t_real_2;
       pmk_yhk          postgres    false    239    217            B           2620    51538    t_real update_dds    TRIGGER     �   CREATE TRIGGER update_dds AFTER INSERT OR DELETE OR UPDATE ON pmk_yhk.t_real FOR EACH ROW EXECUTE FUNCTION pmk_yhk.update_dds();
 +   DROP TRIGGER update_dds ON pmk_yhk.t_real;
       pmk_yhk          postgres    false    215    240            =           2606    51539    t_real id_kam    FK CONSTRAINT     q   ALTER TABLE ONLY pmk_yhk.t_real
    ADD CONSTRAINT id_kam FOREIGN KEY (id_kam) REFERENCES pmk_yhk.m_kampung(no);
 8   ALTER TABLE ONLY pmk_yhk.t_real DROP CONSTRAINT id_kam;
       pmk_yhk          postgres    false    215    206    2841            <           2606    51544    m_rek id_kampung    FK CONSTRAINT     �   ALTER TABLE ONLY pmk_yhk.m_rek
    ADD CONSTRAINT id_kampung FOREIGN KEY (id) REFERENCES pmk_yhk.m_kampung(no) ON UPDATE CASCADE ON DELETE CASCADE;
 ;   ALTER TABLE ONLY pmk_yhk.m_rek DROP CONSTRAINT id_kampung;
       pmk_yhk          postgres    false    207    206    2841                       0    0    CONSTRAINT id_kampung ON m_rek    COMMENT     J   COMMENT ON CONSTRAINT id_kampung ON pmk_yhk.m_rek IS 'id_master_kampung';
          pmk_yhk          postgres    false    2876            >           2606    51549 !   t_real_2 t_real_copy1_id_kam_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY pmk_yhk.t_real_2
    ADD CONSTRAINT t_real_copy1_id_kam_fkey FOREIGN KEY (id_kam) REFERENCES pmk_yhk.m_kampung(no);
 L   ALTER TABLE ONLY pmk_yhk.t_real_2 DROP CONSTRAINT t_real_copy1_id_kam_fkey;
       pmk_yhk          postgres    false    206    217    2841            ?           2606    51620    blt_cair to_id_kam_blt    FK CONSTRAINT     v   ALTER TABLE ONLY pmk_yhk.blt_cair
    ADD CONSTRAINT to_id_kam_blt FOREIGN KEY (id_kam) REFERENCES pmk_yhk.blt(id_k);
 A   ALTER TABLE ONLY pmk_yhk.blt_cair DROP CONSTRAINT to_id_kam_blt;
       pmk_yhk          postgres    false    2869    236    233            A           2606    51625    covid_cair to_id_kam_covid    FK CONSTRAINT     ~   ALTER TABLE ONLY pmk_yhk.covid_cair
    ADD CONSTRAINT to_id_kam_covid FOREIGN KEY (id_kam) REFERENCES pmk_yhk.covid(id_kam);
 E   ALTER TABLE ONLY pmk_yhk.covid_cair DROP CONSTRAINT to_id_kam_covid;
       pmk_yhk          postgres    false    237    234    2871            @           2606    51630    blt_cair to_m_kampung    FK CONSTRAINT     y   ALTER TABLE ONLY pmk_yhk.blt_cair
    ADD CONSTRAINT to_m_kampung FOREIGN KEY (id_kam) REFERENCES pmk_yhk.m_kampung(no);
 @   ALTER TABLE ONLY pmk_yhk.blt_cair DROP CONSTRAINT to_m_kampung;
       pmk_yhk          postgres    false    206    236    2841            �      x���Y��:�E��V�6���	���D�eaV�_G��p1������ ��,y��I�v�I�6i�O��q��O�����
�m���@�>�n��wau��k��I�֌U��/*K�����rMq�8�4�ֻ��~sy�AVܳL�������U�o� 7�����YVr�O���7E$��(�˞v�����GώX�<�\�����Ui�D��?���~�+{���ܨ*�	N����x5��b�{�U�������%o;��&��r�R�E^%E�t�a��d��{�@4�E^|y�F����t��N�e�{���%y�O�t�r�}R�������F+\����0�+�~V�9�Iw���U*):�sV݈M{�wh�4���ً�0a��$�;Ϫ�Hr!��Mc�7X�������x��X�
��om�s�+TK��42�8aHv��(,S�|x}�~<(��u�� uq��a�&ӛs�a�.��b*l�"���_�_ƄU*�ˮ�e:�=/�Xr��Ǆ�����,��f�u���vLђ����K�4kedCc��OX��2¹x��%焯n���c���j.�kI�6��s�h�˂(�-hoY�c[~��GX��Ɍ���G?�%,뙝�c4?�
Sѐ�C9�"��5Coov<���J	������A�� m�Uӆ��p��>h.Ə.Wa�YI��޶�pQt��0��dI�
mW1��<���>d��톬���v�d;7>�U�V�VuJ����X�K�,���Q^��uN�OP����:5���>��֟���dH�W���J{�.�vl{êx��ط~�����Q]���hܹ8:.��ՎWwM��=l	Ҽ���Ӗ��4X�&�z���g�TQ�MF�}�=��j#`a�0M��
�J�<������H�*yқ�f/5���w7;�=OTm�ۛ?�hS��1]7��L	�q�"�~�����0�X�X��
!�ҭx��0b���Hؙi�`�Xua��hf`:8�MN-�`�o�{F�Kǥ�l��/k�~EKȐ��btol\KC Zp˯�R��JL7靉&�x����agE��8b`8s{�4�(�9P{�OD����ğ1��sJ��Lѥ%h�)]�� 6ǩ��NO3y&M�c��gX���������, X�躟3׍TYdU���d@�y�o8�2y�� K؍p]e�e<P���m����K?��
|a��F�#Kۤe�d0��/~Hal��@�v�Ti��2�O3����z��u�sw�hr2�%�D?���5y�w~(9�0�&����`�2�&R,��n¤V+Rw�ԏS�mR�VJ( �2�&���L�H�{wu��
S���@Ǜ~���<�{�4t���wdyN��^Z��_������@�N�c=�D��OAC��$5�'V�,�Z�\E��z��<�������nM&&��eӶwԕ���z:�y(�� ~ �\a�2O�(b�)l	`oX���aױ̜�{���g��G���8T1O�M��nK9�L�%a��m+�L2V$��Ϧˑ3�}��A�0��!�3P �Hv�r�Ŗ���7�R�2%x7�&���c�Bx���eVH�o�/(��Si���tw��w�s�\I�����Il�f�5���&+��N�2/?$ˊ�\o�������h�=���	�)1Dg��!��C����.� ���m�0/�
���S�,�"dB���n�IՉ�,I]YH���

����X8�-��jMl���I3���RǍ�±D�F�>�55��<�7���_v�$+��m�VP�'���h;7�>>{[i� ��X��a�e�<k��"J0G��@}{�<��7
�����C�����}��B�#D�7*t�����_o��7�El���|9x�``�	N��B��2o�v/6u>S���>� 7ع�}^���ۛ�~��:,��6*�o�i����2c�9��FvFU�:ۿ�ԟa5�v���s��yt�1����s���\��-��x�`��d����Ȣ`v��?X*����,(4)BK���b��s�
���ם�Q�1ð���v��l��c݂�gWAu��H��f8t:�eM��t��v��BE�N���<T���s愽�(p��J���4�
ǧM��w��
�aSSͽz���lj6f�u�lMC9H�.0�����4-)�`}�p�S�U	�'�B݈gH,�Y�γ	x'%.L<�<�yUku�'HI��?�9������K�>�װ6��Xơ���g�R�qge�5$���1v���fI�(�w0���rP�[wftO�p��M���<��gaE҄C��&)v�i	��uC�(ɟ����sL�%������#zv�?XAf��1ؿ�up�H,�]�z8	�z���V���Q��Q��4�6G-y�਑�v�}� 6�,��?o���15}�hd���eU8��?���,��} 1�fL*06h�W�fT�a׀���㉼*t�m�$�&�ҡ���2���0�f�
�7�ڀ-��J{�M	.��	)h������Vc��+����,���7���*�[��{��^i���V���j��:��M�pN�h]1j~�;�����%*z[Q"��ް�DmM�g-bk7���3L���Nǋ�Ϋhy����(Dy*M:xs$oъ\����5��c/�z:/6�D��^��5ܰ4�!v�aw��y���p�g�v�w�0�&P
Da~�y󶻸�r䰿�~�&-^���d�]��y�7>�SX���~[A��y5Ҕ��_{�-Z�hn����滨=v�v}K�7��/�MD�~���hO����6���
��?��d�.��w�gq�®��4p��}n�e_��~�K�4<��bԲ�sa÷\Z�\ֺ��hms^+l�e�a�S����
�.�n;*��x��M|w��tN��}�{H���>c1=;];s�`��eH�y$<c�I�Y�oh�G^�C.>���ٱ@O�����,�c��6���Q0����!�-��H�¡q�ng'�9�ڠ�*�<Raw�𡅁@P�d����Y.��ߏ�ˉi�[�o��Nιi��?7�糬Ώ�z��(�%�Y������P��9p*��x���C�#�lE��rL�wX2�,7�����(,�j���w�jY�x�\;�~���M�FJUd$;�{�"'��8E�d�'Ut�0���Z��XJ�P;0�L��A���7�V���F���4���4��=�N!���2fLZid�����m��TJ�u��vl8�HzAZF�'�C��b�i�*��}7���x���4;H=N�Y/��_B8�����/��������1Mل	�_6Ao�}���Q��w?ro�Ed��=��E5��ʂĹx&/'7z�`HX��@ϡO`��/q�"x��?��Ŏ�d�D�L�:^��;����-C���N���F���&�r�X��ߗ�P��m��f��u?��r�]`3��ģꝖt !��͑�a8އ�x�r`�5D<���q��(���3���R�
��/�d�Xi*ŝ�4#ܹ;ۺE M���t���OMθ�ԝ���Y���ٝ�K?�M��UP�)�4(����������S�3,�6o��9X3����b�|	��^��IU�^�VdC�1<,��/������в����$z�]��<��ࢍ*#H���w/��L9���em� '~���VY
�1ޠ-?��^�6W��{����$��K�)�p
e{,$�5oV�6�#��%�Y����0P��q�׎,�
#6��6<�l��R!h��P:*���C�9t�B�����d�O#A�I!blHb6;w݄+��0�~�+5����PM��z:��UE �п���:�چ�m#�1)DR��ʽ�On�x�)��u�W2����|�&�&O�3�H!
٩�F*R*�3�FQ�B(� ��o��*���
k�`J��T;,�>�v�L��?E�z��@]�`m�J��,�h.t0&�	B#`�h �>���+�{�R��\y|�s&�`~�й�G�+w�Q $  ������+/D�
dx��;����fb�9�n��*p�`/������ F|4G��^�2���)R��9��d��/�c��<j館����nH�'|�8S�(� ˍ��`�MoRb�_Y�X{��WxaFJx&����n�[ړ
��"����>d&f͋ �~S��[t��$X5��y���~�	��p�c��i�'ޥ�eŻ)
��1��a�Bv�ovä�!-Q<��ӏ��u		�f�eB&@�Sq�������W�Y' ������ƚ1Uo����!XK�`�	�b�>T�t=���;��!Q|G�0x;Q�
�J��RXC}����c��eu���ba�Ҋ�`����&S��iC��Y��F��x�Җ�d��lK�\7�XI�l|B�}�i�|��������}5�������0��y�A�w6�s�nU�y�� �:|�do��X�&�M#��n�/�����*��A?�m\���6؃T����X�<����-�]�A���:.?�ཀV���=��'< �Ny�����A+�r�D�QThg�,l���*AP?��;m�&��~�X��z�b.�B~��6�ٳ�a�=�L�dJ�����R�� ȇ�+(�s9�ξl7����������I��P`x�s �'�Е�%���FƳ�U�9�ls��p�[d5ٗ~�阡
Sdы����ƫ���0a���(�1E����r�7ow��2WA�u���&N���oG8|MB\ B<�Z���5!OEX��sm|��rTf�BL{���K �cng ��|�M�d�`gR�����,˛��Bq qڌٵ�8*aԐrz~ك���䂜w��1�J(n ��m^��tK�m ��^�i!e�QM�&����r�n�&6
*���u6�OX@!��B |�ͻ6g�Ke-ɷ�<��I.���Bq�6��s._:�wzЀ}�q�s���;+D�P�n����\b�@^4�C�wV$�Q��l��ο�Z�0��t�� ��ƾf��æ�*������&-�� lۂ��n�W�ן����-���=�����[�Q���vR���l����ىLX�y��P��:��)V�F��/hK�~���V���s�
#�
M��q��֟�S�����5��ΰԼ��rR��7�t��#'�1��S�� P�aM�f�EuH,��ˠ[���|�I���sD]��u���I����ǻ�/��^�5�U�T�,B��B�g��ѵ|y�x�c�C!$�u1�n���!�Ru�I�n�\����@0��Y��p=���::Σ�	�%�z���mޱ���#�:,�� 66JH�K��_U�-��Gjf��z����&s�+#�V�y��	�
)U�s\X�t[����]��%�g	��(�9�¥��Qɘ��K�!�Qd��u����]��ؿ�
�œU"+*��RU����\���_�}��&z����� R瞫$�MA:g������´v�&{5A~u�����e���h#V�7���5��7�E�������?���D�      �   �   x�]�[� D�oX��	{���cPG#׿M��JEK/*��S�ϥ���V؊mv8����`��|�|�|Myyy���g�g�g�g�gYb��nop�V�`�F>G>G�@�@�@�@��`��}N�91/1/1/���<w�wX`��}]�[�_]�2|�m�6��s��y>���k��u���};q~�\���J�/��Z�n�.      �      x�m\�Ϋ���M?�~������QE:����$P���N�������*�ѕ���эMu�����Ε�:8�꿷���|5�v���Q{{D����m|{o#�G��q�e^�KSeL��x�=u���}�-3���b̰c�n��
)���#,�)v�?��ь�sFS ӎl�f����[.F�;pX�ehiٵ16��v������Q']���j��\�j�d&�76��S�ˍTL2�����7^)UN�	�}L�.��QL.���hzZC$,�Gb�K	T,�����khWj��]�i[�X��L&w�<ֆ�m)pL"�v�'�b�I��h]i����Xzl�u�2�Rꐒ-�Z����4U^�P�YZ�|.ý!�ySn�v����X��#x�j{ -����0�. �.���o�a!N�(��|���S�sM�<]�s����C��a|�'�
	&�h�a�����S�X�ӵ��z-,���T���^?���*%b܆�l.͍�U�C�?�v�@���v�.�^2ɘrz�͕���>�s�~�t6KOg!�t�0�Ң-$ȱ���Ǧ�06<���4 I�7swϦcU94dɡ�4O�y9�rU�kd}��d>�>(gK��5�h'��Y��O�t�h�T�<�fG���3rJإ�x����u�t�"f�� k��F���~^m��R�;�D�]/f��[�8!�6�J��׍'��l}i�&��a��B���9@h�a���O�@��D4��=�E;��֔�VP����'X,�C,� ��r��p*h���l�C����o�p���s(�f�O)c��2�`z?�v$R�T�O�氩��_��b|��4 ��	!9%@B����O	�%�<��@b��Oy��Q�;����4*WR��L������3s��/P�?��%b��9������alr��D���}ц28Y�h=L�c���]��y�#�)ط�"��>�����K���J��ܐ�ÎC�嬽.fҐx�9� ܚ�0=Й�dk�.�{��:AM�>
£xǰ�y�N��`�|=�bYy�$�289�PT�p^ <(u]��#COM��"�ҷ�t���~]{JN�.�}]摬0�\��7��)��X���͠�K���;
��q K�p>��E����4v��>.���h�qtOS��^��s�y,��U�.J��^u�%��mzM0!���#?_�(�����DVG����8�r�w��c�����6�BL�&ɢ�IQ�f艁:�.@ņ��ED��*m���CZu�]���!����;�hE����5ސ���}	���ȕ�����c⸵�FZpϺF�]��q�,і���8&��͉��-�c�x��ϲ*	?�D���L0�L��?0y\_ThI�4|w�+���=e�k�y&��IC�E�Pz58UG�a���?���T�3�*Â^��;BWI9�*ÎH��V�C����p��F^�H�3r�7yiP����J���gs�b��U$�Q��.G��L]Z<�(ަ�m�� �.���\��=�J���Y�J}uk��4�ϤTdN��O{;���Oq�i��-�Sn�D�hC��	����ɍy%�0�Վ`5`{r��a�qn{p���1K�i�][�5�`�
�����E��8U0���>�H��)K��_d{͇^�R��lH��_��(�kl��k��K�nl����A+z>�����j�#�"y�N��>T�Dd;#78���o��7Q�L�{R��+�Q���v9̟�$�&]G^Z[�"�QD��F��A�M�_L��Ǘq?�/�\ֽ��V�������H�S�b��`�R!�8\�=)~Q�v��'��Du/�mNM�!�z�E!4����@�S���^�
�8���F�2k����<xzP��Ԣd��m�O˼P�.K�$��}G.�e��7U� ����� ��}��UB@~,裺L�\ᑵ�:��y�D���x@��Gs'N���N,���vPb`U��i^���2d	U製=)�
�	Ň+?�a��D�*Y�7pw��3y��~n.TH�N�J;U�7h��UHA�&T�߀#�V�uTT��pW��;�D/������3Ღ�BQI�=x�"��1��`=6�<�j�Q���|ѷK�\y\�TT�߱�+֨�.Y�Қa��}'�^Rg������!����(�
���9��|AL����y@Hm��)�w���� �
��n���,�&����y��l 6Ę�J����'h)s��%��;�
�i��[' ������E'8�v��	��#��rA�q����ʘx$��c����Ϳ4 ��0z9G&&Jb{:u�6�c5&�lI�r2i�D�a3�[��h/-U�R��V����'�V1�����ˈq�R��HTazE�Y��B�?V1�i����f5�ē2֐�^	5
"q"T�qiF��T	^PH��GR]�4�):#Β����g6�o.�q�������Ʀ0$R����{��a(�8���u*��(W3 �S<�yQ�B�b0D��m&"�VN�1����4'Ov_�B�d��T9Hɂ�\2�r!<�?�vܙNb�͙2I'jX��:@��a`DA� �;p^Nd�TƖ^�E�Qq��O<��o�����>��R�>D�r�O0l{�����3�D��CLn���4S��I��7pn��c�Ź��v�-tT6V���6ܛ����>����^�1���4�Æ� z!m"*$�����Ղ����s��6d�������tL)��ް�	q&Ң�ǘ��xJ̩�G#�4Á��i����.�\9�:P��(�B��v����`@�]4�QC���8A�k�p�C逸S2�MKޅs{wp1�J����.*j�.�i����]6�c7�rD,DG�E
�$�XK���lx�!����NE�磼��(����fC]�;��L.�G��v�c;p��L22TD��o���DU�x�ߧ�젧�;J��|6����-3�	��OoU��IY
`���m���j�|��ѹ��b�ks�������>K�`h�9?~�)�Y��B9�= �HU2��%���~�S�I��v^����I6��*"��RV�ۉ������JN��ɡ�i� �n=u�q�@��K���<������ 0h?�hd{��!�H�G���B�V��]�|�aR�(���^�{n�I�W׎#��E15/�z�/7�8�1�h�
8w#R>J:�༺��&�1Wt�'J�0�l�����9(��55��6��O5I��55��K�z)ȳvM-�Z�k�~�$�F4U>(�4�F9�+��4�>�f���xQ8�T`� �h��9X��K�0|�mOT��@MՅ���6��m�ܘ�!������mE9PS��%������p��J��@,�ڍ	�&��
�:P�u����l�~��Wυi�QS�_MAt��w�=�{r���eO�=���,�8�QH�|��H3�fMm�}���D'��!K���^C�M�~��3n)E��A~㓺E������C*Դ�C ���pkm��)Nz �d�D�J��,WB�q� ���բ�W��׍��U�n���ʕA�H�v����r7`�J�Mm BE�B���7ȏ��&pl>�%Gſ2e�&r,6��e� �y�I�j�m�EZ(��l��V(ǎ��x�/!�9pιK��?o8�����e����":Υ��@��B�;J�6厂�Cm�AX�0t�\���F[�J�P��ľ_ �J0�a�|6��HUG[V ��0 \�H��%4��IY#����w�a��ֺa�i�;uT��A�3|8�ڨ%�2�8�������L���AH�Z�I��~��{9ϐ3�#b;���j�m,X��������ӻڪ2��.q����/_�&I��~� ��ʂ�������6AK�5Cޖ|6`m4q�A�r���ɋ����Cۡ[�U�ȱ��R��z��Q�ОK��ֶCB�E�F{.*��99��e�����ʞ�1�]�\Z�խe�|4]@�t��7(�d0� �	  ˊZ�����]H ,[�9� ������$"r�ϴc�#n��#i�����/��{Uظ$7��G���抅��:'z�K
<��u6IF����{,dJ!�����P�!�L���V�O*V��
 |ңၠ[ّ~>LOtT;n�r��2T1(%����B>Ш%@)��Es��L����ֲ��?o� s9Ww "���n��9��Y�94H�|������� \�#���	=��		����۹�O;#ZL�<� V��Nz�T�Ќ�|�L)#��:)9��z�rG��H�K|�v7��zQ�	BTxl�B�_>_~*�3�#�Nn����S���:�bx��	Y�A�{��>�G��s��σ���	�d��ր�P��I�=��s�\C�T��R�
y�S�clQ�F:W��#.�b�p	 ,�T�.Ecj}OזZ۰�"���#�=2in��i��Bz���k.钾"���jS;���{�C�^؂�=�����j R�6u8̀��P�n�9�ԑC�]R��7F	Wg���s�ND��F�j�|O�n��;��{6�1.�h9L��t�{�fqYU�#v��ԿRG3F�6�ϿP��u]@!��٭i��,<��m�<�>�&�}�+��#����h [w��z�k��z#��ZN���逬Ra����q�R�����#�AB��[�/y�(u�RV�j�j`�p"����(����2H�sN�lߞΜ���ϱ}3��x�TExx�<5�p�J�j�����Ér	b6����4�-$阰��iX�ȍ`9�#�����	%��8���FY�EjU���t�a�t�U�1;���yO-�HC�)w蹥fq�9���;r^�g$+�=]�dH�����j�b����n�v$,��L<`!V��"�..��ġ���#x!��@}*lmO;�#$gD���jG5�=wP$Y�1Vo�����w٪k��p��HًV�Űa�s�fL�8(٘��=ֶ��6]��#�>��>�Sߔ�j�OC�O�%�56�D�n��V#g.9����rU[��_w�� ���z
��r�A��b�C�-X�q�a_tщn���.���|b��I�8�qt%�Aj�s�k�m�pB��59xv�8�� ]���\5��=��t৆N<�����O��E���)+��]���I�6���:x��Wl��i-J ��,�x�ϙ���U�'����w@���e� i?�n���@�p�"�4g��v=�N���?`�=I2>��M0���ϧrJ���5d�˃����t��6�:q��9`��@���u�N9�L��~�(y�>+�	�
t��Y?�&�I�jl/�f���K��a\>9jY���:}�N�Jd�&�}��/�qD#�$L�pzts�Q�8+�~�6�3�����������D��V��1Qm(8d���b��E�/���M��q�]������Nz��Kc&��~x�Ȥ��t�5 �� ����*)�YLf�Ρ�NF޸ˢ��#�jSrK ��x�X���D�IA�Ĉ���ss�C1�@���$^M��{R;������N����_�*G"�lD[>�Ss�|(�(���|P=G��(+����
G�٢��jS�S#���ڈ�4��=�xjx�~4��ȑLS��Jn�9B�����'�q���SxEԓYEr�!�!_X��p9�����6R/��8Y>e x�]x&��T������x����|o�a����nsE��I)�rJՀ���|�+y���\��/� >�4�1�.��@^x�-��y�^^�$�1p�=����9�=��shs����S�.���A�b����Ȁ��D��뫜�u�:��/TC�K�僧��������E��{'�bH:q�W��Q��B0h�=��x�w���*àP��]�y>o���k��z�I-�u��ޛ�����]o�+8�y�=�q�����8�
���7�j �,��Y��2P�^Ҋ��U�ER���-;�,�����ͨ@u|�D��<��|���H�@����Kܣ��8twk�|U�VQ���7�����H���v�ˀ�_2	�c� ��{��V"Ũ�T��6c�M�#t��	�Ʀ���R��U�s��o<���M,�s���'�~�!Z���j�i�녅e���e�^&�z�}����gw��9���F➎�ڑ� @&	� tLkRP~Qo��\7Ĕ��t�����ݏ�h�Ű���+L��@I8t�.�ũr:�+���c�~r�V��=��a�SO�k��g���R��=��B�2��j0��A�� �_R�M�j����~I����F䕹C.�K�D㋛�Ӓ�*M
�zѭ�ZF��FGK�󮂲p{��@1��f��m.�&_�/�0��"1vX�#ד_����Hܺr`_�B�!ȩ��q _�f�E�v�����7���v�yڀ�����������)�7�!*����G����	Ę/��Xq�����Q�|C��H�7�i��,Apb�M�(�ySש]=�*4�QJ������?�V!l       �   �   x�U�I� D�5�����?G#�_7� ҬPO2o�Z�tARH!�t����$[k(k(k��l�l��ʃ��� /Ќd�1٘lL6&{!�T�5�I�r���1���6ډ~�}���O�Hk�[z&�ֿؚ�~���_�L����/���)hi      �      x��}Yr$I��w�S�f������Ez�Fa0��"���( �����t��̒
�-0,
�?���y:,o����{_���9w�94��z�1��!�s,��!�������T���������@��|�/���PjK���G�'⧜Bͥ��R\���9���?�jj��Q�~��|>�F�//��>�PBk�$F�˴@!��#/PH��J�t�w>�����>!��g@%5�@!E��]�)	�������~�O��!j�p��c�8B�W1w��r�r���R7[��n����|�/wU����J�ʫ�KK�DH��U��6�l�� u\N�m>�^��0��O�M�'!ܸ�
�C:�V,���������=�i9|�F�4�(�*v�>>�� ɥCr=�f�H�m:-��PZLqw�.�����'ۑY���Zjv'ۻ��t?�O3�Ps�ߘ>�?��ك�K$;	p!Ăe�?�]W���~��=-�I���sQH���d�2U�'�,T��O?ڭRxm�RZ�i�S��M.ʦŐǦ�`�i���K��{>]չĶ��$Z)5H�5Hd�� y��vɯ6�v��~�=�>��'$�O�[t�Q̙w;F���!ZM�̏>�o[��0ؙ�H��o9��p�ml[rLf߯�t�Y��#Z��
�w��P�C���;C���3�S7/�4�X�/7���J����I��������N�ݹ^-���HƑ��+"ì�����~Qp�Њ_D[�/ʙ0��@w8"�)��������I�ǅ֑��m��u@a5�_��0E���y>Gd-e5R��M4�UO��9K;@����o��v$K�z�����n^�E*r��k(�����}?�q���w�S�����?fW;?�t�=�~��ga��\����+�����6��U6���цɩ)%69fd��Y x�n��
��~����T����D����ȗߕ���ӟ�]��W@��w�"�>���c�~�v��)N��Н�;IeD�y��|��^�**�����@ǹ�{�ɢ7�=�+���t�|"Z~E䝸Ӵ|xo���Aq����j�������r=4��b��Ʃ΁Iܽ&�-��.{���w}�C���r��y��}��%����ەxˢY����`�e�=���[���ڢ2|�s�l�(�S��������lQ�CD1���k2�>�>kd�;��y�^r#��k>Zt�Z0�b��ǟ�B�:�3F1? �ˁ0��u�1�1�?-v	�����~��*�V㋵�}��Dz^ٽ��(�{MO�]�(��e���(_{l%I�����ζ�֤d����e��t߂���Y��(���h��?��Y�WS��2�մPN���`���]�OA5�� �ݓ(����(�VK]��D�*��iFK����/�E����x^�x^�7ܲ�����㡓�$��&gr�d�K1p����%YD�e�vx��]?�ӡQ��v)>�4�׳$B�M���v15�N��6��?�F����<�2�����T�����!�\�@;8^������)��x���*٭H֘4���&��z����)��~��p>�鄀�F�7b�@Dﻫ�����O~�<c�5z�g,�g�gY�%y�����B��,�z!�^�4Y&;L�p#4���|=_��WzY���c��X#NRu�I��@���m\�����.>3�9��b��)P�`fSH��m������U2D��h�����T7�É"��pm�V��<�l���*�Gv�N�3sS���Dx����#�b�'"(և�P�<y��N_��Nf�L�o<�]�Tˈ7��!���0��n��7���mzu�C�Y�i�/��ӑ���Ӵ�vv(�u�����L;���SF�G�Pqd�8��"	 2F�PXM�;-y�gy��x<h�d9�H�3����s�dMm���.3�Q���Nu�����Խ#R�S]J�C�j��Η��z��M�y}ݷ���� Q9CL��%C��4�Dyk��Z#�w稈��3Ĺ�<�j�������4/�0�&d6�!B!.59�N\�@��$dZkv	����wzZ�4ҽy=Dd�<o25\��H�Ɇ��݆���r	�wyg�Kg5��fw:ۚ�-��eb�j��E�'��P�]�p��VS�N�������<-��a�=W���]#;�VxVKMN�m:����w�GN�����JY(�꛺�����0���>�ݞ'�j*Q�U�eWtϢ�3�e5տ���<} *eOM�&D#-�r)j�b�+�2��y�L��NF��P�%�ft�.��9Q[��bl��zF���'�$����w<D�,�F�s�
L�]jz��5-�G��+T��q�x�qH�4����*|���B��`w��������܎�Ig�>[�=�c�Z�ǡ3<L�h����U�ş�W?*��(|�Lޑ�AZ�?J �nן��|�"�&���}��h��c.�+Z�lǾ�ns��odc���;�3-; >W
X9f�SbFC��u����r��0'����a�I�Ua�ބaz�h�0�a�L��َt�� 	�������V�^�TǕ����@�x�L������%j�t�:�e�LA~�\���z�v���ϗ��{������6¥��@�QȓG�N6�)hs����i��YQ���g�ԋO|���Z'���޷��(���|��۾�K(�V��6K�����q��KjU8��|h:���-2�^}��XC6K��6�nm��Yη�x�K�\�CT>��p���nA_Z��5��ˎ=S�"���<vG�kod��J�>�i�:Bl��8r��:3�MwFМf�Ⱥ*���Z�Za@�$D<#�<h���8-����^>�L�3B�*�kN�6�ja�ї�3�.��*��P���FV�Q�M󠛗�yy�E������i`l���y����z��Ȅ�,��@ ��]�jx�Ҋ�v[1��;�2.)~�Fˤ7,�7,4K~Z�c���8���ɋ�D,�^=G�t�s)ꋁ$�݋���}/��v��#��/��FΏV���J�P��U�n<Ƕ���e�Sz��P/<"7;8m���owx���]u��BY�҂�D�u}r�4�} �����\5�ݐ.�P��L�GrD�t�]��;��s=_/�~�GG}S�E�CM"m/�9Ė4��մK��a�o�@�qN�%�����I�$��
&%�X�2�=</���i�c�mL��p�L����ݖ���m���`y@�}]��ŭw�>�[�JS�/��֓��/5������4����,Q�3E��%�����C�z�~w՛Ru���}����>�X�:�����T�J�ܦ��WD�1X�ax��)��L0���?�W/��S4XG�\��t�5p���0p�M����M1G0�_w*y���
���YҮ{���t=7S�ȤH�l���rn!�!&�k9��
o�-� p>��?( �f���g~�8���{��nǱ��F �R�<�A:��,�O��̕[r��T����&+@A ��|�~����}�,�4	���JZ�`+�΀"���U~��� -��m>�%B�E���W�.x@��k1�.��1���]��#��C�^
!Ƣ�O:��.T�n����|,�C�H��gn��FE�6*�6�x���r�ݿ����q�g��:����`�9z���!`WF��y�^:Jx��M�uPժ�&B��&�D%4�Pd�Rm���=}-����k�T�w�9���3��T߀�ѡ��.�^�(ʡz
���Rux>�T�䪕�\�8�\mP�����e��Bk�ۘ�_w�� 'T���O7i�{��,7�j�x�:��s�ї��Yu��t�9Gt��w=]����=�ep��r�=���(��?���0����2!��;R-���(��nA
�4����1���ۄ:,hp}ǎ��a�&&[�룶Z���^����c���8m]� ,'\2/�)���)�bh�|x>�o4��i/�(j-l�
�D1L�75L����AZ+r�6e��"�Щ��g�iE���jy��c�Nh�㹽�����U��Ђh�    �'ɰq���H��������E(U�-C�F�ȩ-
�U1�D����]>���7���]6x�hm�H._ �^��WL��鴠��֡�;�rY�R.������h��00�E� g���~�r��MH�h;዇���
��v�?�6�K�¢�<����R��<���-���R:Lg �������kW�>�H7���/�.kz�]t����Hn�7�&��K+��%�+���w׀Գ:�H�Z��hadH?h�����p�k܋�w&f��B1�Y#{D[h(^�.F��ܸ;��FH��;e��%\��;�A�"���-A���r�2�&��6��r�uD8��U��o����c�5���i����b)|�� y�Vr��@gx���Ȉަ���g�����|j�W7k�'$u%�S�vO2���|���Q}�+�˒��t�L��;W��_ �瀎ܺ*@<���z����1-�"�	/k�$1A�D08������ݰ)ţ��}�&
s+���es�(��f���t0ܲ��ɏ��/n�$}��(pZ�S]���GuCZ���p�Y�u��94r�W��_Od$�BF[G)�;6ܴ2���w<j�PD�ad�$��#(�"�����7H_�珙~B�M�XO�7�U>ɣ�ܛ����JFyP��}�:���)���ߍ����Ex�m,�&�ܫ�+<�����Lu���v=�h��VE�6�("�o�QD�}@��/������_}��g��)�R yMZ
H�۩t����;<%d��jo�7��a�X]��;��%�Ώ���Na]��4�܏`����,shx��mDK5����V���>ߐ��U�D���dL�smh<K�9��@D��r���"	��OA���cS��;�F�|D��i���pGDi�%Y����ZT-����.����;��j�W��}/{-��	l@��uh�t;&����t�Y8�����ܨn]��	ÐLbP�aJJ9eC�;��4/�+�&H�e����t��5�|R-�ɍ�T��(Yd�VH�LB˨�Ǯe$ $ĥ�MC\?�شfvLl���wƖ��q�lY���[6�|�RWh˴S��pn����UӦ̚���S�I����JAU�jǬ��t]�����5��*7]=a�� ;���5���f��z��(ᤔ��M�E�!�}InR��_�Cb�'>�\w_XK�h{��͋|�����|��������y�����>�w�����	浪jY��Ѱ���B:�+�$���'���F�$ Ե�!ڱ�h5�����"��T��D�yIH�a*-��\}�,�~������������&�'�4w�IEM���4w75��<`-_h����l.\�*) U�$bFQ%|K����_�,HJ�[ː�����&%r���6�CKH��u���,7g���F�����F���� ����lh<K�����%1A�?hbBR�x�{4��%
���Z�����d��Sl+�:��,֝�f��(I��o�Q?�F�U������6��u �E	����ʂ�/М��y�F�(G��")"5[�Y�&w����}����%���^�X�5K+�t�-����v��TA�*��rD���m��QbZZ�,1-�"�'#sf�'+M ��/�e�U����j��ǉ� n�I�C/fCW�tA�����~���!+r�����ʚ�3�.�yw �ٮ��@��~\O�!�l'�G�\�����<_�C�� r[��]C���td5��	�)��S�鑕SY�ClW3#5<�w��C�kG��盖<����I��byӪ�i�D�]sY����ι�B�u��_]�Զ3D5�{v�o$y�P	�H�eBU�r+N�K�z�[V�Rc��H/q��*�^|6ρ#"y��3LdU5�_����HG��.�f�Kvf�(au�]�j��^1�՗��q� E{����."&�2�*r@����W5���|p{Z;]g��>�=�CT��p �j�&⫚���z^n|�{lm��&*�.	��!ξ%����H�	*�E����v��eЫ�lURx;�3,@45�'
dOy�;�_oZl!��A���k�+ۯR�k�B�p��M�E᳾���H�t�r�{�D��Y��D�9-�!c[3���Ȩ�:2ꭍ�zu����k��p����w����dfS��B�*�YG&�03۲��,���i��ж�&$�:T�4[u��8o�FE����~ę�iH�m ���ج�0Qi�Ӣ%5�U�3AD��}�H�]�F��1	�
o{Ĳ������ >��Cy
9_��Z�U�
ƸnL�-6)_S��׭��44k���%Vl��Z(�&�g�����n�3v7 �^1�m�+Q�)��O(��W�JM�^u���t�N�e	��6y8h�5�����צz�,�o*<�	Q�i�:�]�<H�#
�Q���*J�Ͼ�1������E��n���6�nY�|]$[�0�|�zG==�����:U#�a�{LQf]h��ds�xV5h#Dy{����=������7�h�͎�g���)��!E����v�8�����m�JR�䯅��hp�T� YR�{�(���떢��miI۵ �*�Z��.��a����{F���'�53�D�}/��ex!-I�V�d_���Y��W����i7`jC����+g���\����{�̔&D7�Ƞad)#�$���$����0��PN|\[�Y�(�����H����g�쀓�W�pM�5'�
/�,�y&����5�<�S�!��/Yn�d��Z�e3�^�*��,1vs:�U�cc�)L��fU��{1L��X�J�߶��c䔭���/�gT�gK-2q
��(\t��_�u���2}�/�R"��˶&�C��G���Emzc��n8��趜�#w���c�Ť���2�(P�=ǆs�\���H{;�j4�|����ߋ�!NQ7�������$��H:�ф���+��|�6�e���?T!�1ɚ����5��ݺJ�xz��rmg�%�w̱�D|uĪG1�.I'b�m<�0�����$�u؞.S�MYTx	R��K���$_y;�q�~�E��RԹҡ��� ��ᛘyא�(���Ζ�MfK�[K�`�KeמA�V M���ݵ���6.����TD�θ��a�!�AD^y�p�J�9Gi$pSed�Jh��2T�|D����޿��C���� ��,��/��B�
X�/֪iahn�E���033bP^�[([^�&�n��t��Y���#D~�h����s�u��1�uuH�e�|h�|�}�]g��@pV7I��.�DgFO5����Ry�r�u���E��c�v5��Ph*�Nw��\�vr��T�����]a��ұ>���H��F�$��I'�hD"/�0$��Z`���?�z��/L�Ez���}�t�D� )��{���運k�#%�����$=�$��ȓ34�!oVj�@�H
������!�֫�����[ۚc�~������
�W�l���m<ې�ι���_�u�>`�܏4ٜܚMG�����/�JE����%Z�Tۜ��H�?�꟪���\���GIt@��<.0ת?����d=1�i5���O���0%�ܧ��ܻ����w���:��{���,�3��Z�0w�r�11�����o��H�G>k�T��٠%Qb���2�`İ9W��Ӝ�����3;xU�FV^��V�z5�Q�\>6?�Q|���)��ҐL�J���7N��O7�m�� &Ֆ���6%��_i�GJ����( 6|��Ʀ��OY�A�}��@W^�@��������b��>F�
��э�d����N�JI��#��F���^�Ѱ�%ĺb��BӍ�f뾾���e�@c΋��w:g �9�# ӿl�D���rwi�ټ1�FI&:80��8��k��WH���|�n����8u�I���D�GNo�ai�h �]{��\��^]�4�	\��9����������d ��n0.y������6�sń���Lat?�O`�6����l[j1k �6/��j�����~���Q��� �d��xӌ7�b�(`v��'��0b����t�    U.ls�K�0F�Rb	�N��.��R^���ز�MO/ʈ7[W�	��#���nSz��tZx|��	`��qU��w�f��5K�Yv�o��ޯ=m���������㥬�@)뢭D���at�ڀ�3}�EŜwJ�h�g���Qt���� �e7�N� �;���r����B�t#�#qy�]��]Mv<
r�"�J\x 5�w��$-QWI�A��i�9:�>L�ШB�W��trU�&�` 7�` ���m��eL���[Ӡ����VW
n3��9��C�-�#�Dg�*��Ӽ�|f:��r�� ��N��ؗ=OƛA&!j�f�����a,����<���Vs��eR\��q���Ƒc<6�xÍ+��+���^8DHJ��]��^��ȭ�k�@`�b^ݘ���z�t�FA�LLc�=ÆaRn�]o׳d}t~�����ܔA�e_U��9;�i�}u��_�����v��uJQII���5'����b:J��PDv�7)dI.��K8Q: ���_W	#X?0�s��{��jP�=��������e�B	���͂N�W�2�"s�e���5��hX�,q ���t��0����"�m)�*��1@
`�y×���|Zd��y6^Z�R�YZ��ϵ���V�k�e����m���R��U�L�H�-���xٴ���j���ۢ�\�9�czڹ&��)��uڹ�عb'��j���p��v�,�3F�K��K�S�&`H���e��?�|y[T[���Z��U��oU���3䙕�z�s��s���z޺9&_�1a�b�Ή@�����T7
�żvҲi���.�J�
�deh��o�m�g:�x�v�t�'�~r�㠞Ѷe��]���Sʧ�^�_v<D��^�4�n��<�̠�#����}����.�c����� uv2\�1�^�ܠSX(U�������l2�T�u�">�lB� ��H��bUi!�H��]��7�>��i��3�F�.���G�W	5��؅u�P���Ɇ�W��O�ld�Y���l�#�.U��TU��j;Xl���j��:-�wx�<]��٭�#$*�Ȃ3.�W�T{t����m׋��Sf8M)�2�Ő�U��w�a�^Lʰ<�ҋ�}{�ן�׍�u�3�ӌ�wqԩ�{@G_|\�1��ѯH��ȶ��D3���u2+b?"H���W$�+Ժ?�'fT���V���pD�j)��ǧD���'YSz�Y�V��^(Ž�l��+H�h�tdb�L8K��-�<$;B%����(��`Y�ii&���C��۫�HXw-U��k\}�<M�}E��˹�iɮi��Hp #�e؃��Na1�������)�5���oQ���xd3-/��
PU@����uh�RyZ�.Q%C��/?���}:75�f875������� �u��O����G���x������D#*�I|v:;):��O�~8�"��H뻪�FM��5���H��jx̻{6����X�(��4/�N�rQu׊_uל��Z��e/��	<.���rM1�����p���v���'\��,74Aari|]1rQ���fϫm��a�����0�Ĺ��O-��0Vj�j� 4��ߠ=d|w�x���/��~�gԪ������������*̋4���y�^�_��T�UN��w��Z$X%k��B�Ƈ
�����Au����ƓG\fq�gk�|��u�RR��X��5y2v��B��]�Ϡ�f��݊�$M���?��M�!jD�	���i}��M��0*T�%`�UY������r9��w�K�*�Q��	d1V�ŝ�t,ɫJ��]�V��C�6c�����,��9-#盢E-E�X��&:�=�pA���NZ31��;2E�,2�Yݬ�MF.��>~���9_��^K���q��K�FLl����v�_t[�~�O�C��PG�
G6��?��t �ȾEg(�F7
i����rR�&����X�ӑ����e��<�!��"�z�R�!�sQ���]�S4��!A�Tn#He�t���B�1g#�FȐEU���v?Ěi��x#����-(D0�B[Z7� U��L	��<�캓��뻍���~��V�ܹ���B.Zt$�>4!۪ J�v������6�'�#h{vj�yLG��.�����늊�$�VH��
>�qrC�+���aA��F^,�n��ol����5x)Y=�v)!a��h�"T�]*�<���c��h�;�ݸݽ�d��ܪ@ �f�HV�ٍ������~���P��
_H�G�K���P�M�Pm7��?�?��k��X�˽S�[̅��ױ�>�o)�`�m������3m�N^�4Rg��t��Tї���F����"j5�U<���,V;�2��+��r\N<�N��?g�	U����1c�́]19��B��N��Ȁ��n
�1�s��#oFY��6C�$������3���lx��1�ƓC�}��A܊�m��p6��dy�!(n8�[:�?���D�y-�U}F$�x��(�L�����i������W�t�Y#�t��@nq�tz*��A��v�O8�K�6�=�0%	����)��\���(6v���O&�O�w�����M�$mJ���6%'f���jV]���8ݘ�k�ѓ(��2�u$�]�	NN��[���TDo�j�ݻ��͑��#�A��1��G�v��N���<��oG��D/��v�8�n��ר������z��H�+�ڝ��[)A:(k�J+��e�׌��TP�Lg^�4�H�S�F�".<���%5gW?���TD��LQA�<���퇆a�Yz���!a��p���$���Y���m�a�(=��M�, ��K�ܘ�i(����L7|��,iC��N�I��H�����up��Vf�)�X��ӕɷA[�ȷN[�B�y�� ���LƴBz�O�đ�v�7�H�[r.k�M�k�M3l��1��n�o�-���(y���͡�F*�Y�X�P;8��ܗWɡ�[�0��3���Lw��s�u����w
lG|��Zȃ�*�*�rk!��L�B^�큉����9������µ�x�;H���lGv������;Ʀ�-�OI�6F�8F�udA�v*x�9/�|�=y��Coc�(j���3's05P{��vs�#�p�t���Ö�nu��'s�B:���L"?�pu��!gO?Ϗ����8�E�s�K�$����f��r�����3��]W���W�)�pT��?�p �Rf��M���-@�վ^�M �.-	 �G��C����}����i���9�{<2�*(�����0r����	U{to��m�V��QZ����hΰ{#��Y�����y)�6o��BW~�������":�L�6\>ddImAgml���e�S�B|������7�n��@M+=�}C.�(�ӌy��R�شӝ���/�$���4�J^��R�u+�h���~@z���{� �3AcS��:���.�'r�~��qkq7���#���#��Љ%�Ů#(�8�ܮP��}��M
 *; �zL��c�M-�3��]
����ܣ�mw�Em�D�P^��C\�����虬g���e��sٍ!!�d���z�<�R����N}|���k
�L�� \o#�Sox���t=M?(4/g�0�~�V�!>�g:ÜV�i�ũ���B���|�A�!���	"��!�<rQi�o��0u��#�.�٭����"?̨hj������C�A���oiOne}�E%��1��6�3�%Ӈ��q��7�\)�O�r��v�G�u0/��*���ޙD��w�%�v-�1��p��u�]��1���I�jk���Wo�&�o+Ƥu2���R�v�m���F��X���y��Tk��1M"��4��is�I���<�+7l��,�6�����Iuy�I Be��!T�]�J,��i��5�oi)p����������g��!���(P�G�w�C*|KZR�<D�$a�'��3t�J{�z#Ǎ�'��ԭ����ބg��J��]� ��B�sef>��)?�9tX��N�8L��|��MuL,����6�    ����M;'����^n�;a�o�i:�qD�;6���%���5ioc��w5< a�r��?�J��\�oEs��qˋf��H��i�/�HK�*��"S�7v�.�%Ci�X� 9��34���kk6��ŖlRh#�0��΋�Y�\/�BB[��!![*�Z�$�r�t$YyÑ|���y�m��@��vk^{j��j�mU�m ?�N/�	]��8_��V���ħ^m��C��ƶ1BUM���a�6At�^��. �:g2��f	��V�����-�P_���ɶ���pj�?�j���,�5C7�~o4�{;�Rڮ9�#D�F�U� ��]sLl^м_Oo�ӡ�sX
��+� ?�Z��I��6�h8�#b�%߰��F4���A,��A���#K��A��� 65�_�׿�q�l�O�"�R�$�$�UU�% �6D�����1����I��e�s[����&����Gy����~'<�+�p��س�����X�|!+#���Q��F졹�ܮO�n�P3jTjFЕb�V1dض:��T�j�%�:�B��D�K�ЫyԮc0tȚ���v=����4,EE�2�c��#u��H��d����Z��߹v�_[�<Y�;a��8+ݶ�q�Tw+���j�����)�A��T�y5@(���W@��^T�(�01� �"	M��p�,�HzX����jP,_^��TY��"�* e�G�8۵p�7���� �[Y[�\%��
���-��y�aFO+������Z�i0��M�tiXnl�:�|��K4�`��W�x���Cﻗ�:\8q͊�� �U+��̈h5��Z#q���:9���6e�����z���=�K�v"0X�Ҕ�+�D�����LQ�!����YBm�-�}���X����=Y&U���*�����e��������:Ҋt�:�J̤7T����템�;J�epi%P��̥)N�+<HqeC�v����A��~��G��P[@m���5�$YG�[��O�����tZ��E�yP�-�,��AS��ˌH�h@m)@�\|�����4��%���f(A��L,v\Ls��%��s ��n�������u�L�(���b������i��Εt�+[<`
�PF���9�nx��
i��_������u^蠴u��g�O���U���߉NB�>��7��.բ��@Q��#��@3\�6 ��o�ZΥ��Ǩm�:2�2|��f	��Hv�|Qr}�ow�������gr����( OT@��w�7r!�ɷsҠvӣ�t�S���cD2�߲��(Ѝ��8Pv�S���[�'�S��t%�m���!��k�U���NT��PE�4 �����%�q�8h{�_�iDY[� w���gC�_�i]���2�IaO��������5��vd7�u�.�M>L_hB����Zeg�Z�&�&
bh�(�ɶ�nG����p�_~X�}�N@R�v�����Sγn����4��k)${������"�w�	�O�w�*�����Y�U{�z��޸�7�g���j��Wii�3�-m��v��������O�:C��'�� �H��4�;S�+���/�~k�i��&�r,*�Ƶ4C@~�oW5�U� "<�c�H+yǽ���T8�#
�#"����.�`j���1�װ�)~���*�S�v��Y��2���f�d��(�����r��c0�_#T;�/�%<��}�Ҩ��J�Z��t�R<�L[��LH����z=
P��`�n�'s��Nfr� �¨4 7 C�ܾ�t�~�߬���#�����,�V�N���.;H��~��8r��*U����/�+%}�|�#n��6��2V5堓�6�o	#��$}[n�t���R�Lo����y:^�HŜ��\������Y	�B��Ջ��/��_L	���՛�T�z�R*`����t�~{���|��A�p]�M=��J��t�`,�h�7]��1R�-��>�^��G�7�p�jg ��0~�dϯ"[DĂ@T�i'�C-��,o�*��K��D�)v�ю)�*0�����rgM�f��f��`����\������i�[+�t^A�Q����o>�h���h�y��ǇL�f��g�@g&�D.
htRv�c"Wtv�(�\a�3���^��1uv
P�C��]�>ܥh�=�b[!����,����.��E\�޻A��&����w�)&`�*,Az�n)��A*,��޺��|�L:LPG5�YC��b�6��UW�;_츓�f�����睻� ~���a��&3ȋ���0��2���/�t�/u�V�'-k��!t��bX�Hc~��@���ץ�_�1�[��ޭI�f�dM;�~��r*�1�����#c!�<t*��7;Z^J[�yck����>N9>�/��1�������aN2�GU?��WF�#u�R����'&FA�d�T��i�� P�G���y�������Y;���W\�	�S�e�&w}ձ��k��	X�P�'Tob�w0i/�S�Jyuy)U�����+���"��/DrBٱ�Ac���=�T�1�ސA��^0����Y���O�Z�K���O-�k�qrO�ޯg��pzv|��c�3y�2C�En��2�@�o���~���<�9�kNIڜ���ќFs
�z�JPܝ"5�Μ{�l����,c� >��T`�,�a=-�-�;�38���K/��܅�����(���W��?�e��6_��;Tjk��I����~؋*CSe(��P��/L�G�����02��!��갤���D��6�� 8��!�d�U���C;Y�r�vp%�A��А����ϧ��u��x�(t�(� Y�R��+���z��1X׷�v�w���6Bn����#D!uo�6BGa	���=vw++k�袆�C��-$J���3��O:~�)��:~�q�ε1~�Y�O�]��D����/-ŏ����F�=f;�*i�t�g�{%��v�s�eU�u"��D�خc$�<0�ˑe��إ@Su���m�!ב d�G�c; ���5�޿�	Y}(j����i�[
��V����y��/;W�V�Z��簌*�˖e����tĤF�0�j��i�5͡9�>.]�v-���֝g��@}R�x�"�N�F�K�:�R�e�R�T��4c�[��P.Y��'}�-3Cկ�n��9���{��Rt)�]��A�JٰFSÀt���;���VȠIY
�(##�%�
~!���rZ��K�YqE4Zt��'L��N�HO��Ft��`�OxMP���|���*���*T�s��?B�
���N�ݐ)Q���C�*9����[Չ$8R�%�4͐|���gy��
����ӭ��m��QA�Id�JYA�B"+� ��S�+�'4s`$[�צ- t���dOIy��ۏSm+����~�ƕ���sJ�L�Tc�֢�G&\I7�Y�nj_1������J��G�I�|KU�-U�ui0̷���~�F����WV3Yt�<�u�
��_J�m��-H�c��+��8Ll:���r�������P�bYI��kA1}�O@�Ƶ�k�"�D�>(�Iڇ jڇ̓a���4
�~�Ƞ���KJ_DHPx�IS�LqᔏR������&N��J�z:7jRMiH5�j�'kY������&��w�B;YC�%���!ax������z������ѯ��~��at�<oa}ޒ�����|_9�[Cy��(Z�p��4-"ٯ������m��L�t�~L��AI��|3!LH5&qJ��S�8��xy;L��iZ��{�5�ܬSY��tZ]�Ɨ<��e*����s���.Ni�,�tڕY�j�C����ôLE5ߦ�����'��%��I�n�'n�'H��mFe*����A�б~�3A�X�HZ�8�����Ew'J���|U�xRIR&[B��y��?�
\$�v��zbvFKE?h��Z�����)	~xu��h�����aWm���#FQ���d��)=�@�;������������l���%K8U�`X(��p7v9����2yi5ܴ�v�� 5�> �  ��)�ޑ굻R��]s�d�z�Q2	ݜ�s�n3� �'&J�6kQkC�HLC�6�;���*_Z���!wHk��O��	��H�)(n���zX��b�!=�*�
��jKT�Q(ɐb�]0��x�)��!d�(���(!��D�X�<d����z�(��IL�S��Z�<�̣�z͚?�����$������޵ځ��9`6�l�k:r�"���~o��;_�/&�9��,G�v4E8��4][�]6�>A��}����axF���eb�?��C�_*๏
8Fdہ���~Ynw�j����TE�(�:0�5S�z��"5��v�~��>��6�{U���tb�c&�x��J��u=D��³,S��X1V-�*)��Dj�!��Vx�Z�# ��ֺ4��-�8�![V}�&d@Jɮq������_��Ot      �      x�����#��5x�|
^�UK�r	�ȏQ��"bS67��iR����T5c����=� ��O����[	��pw���wMV��%�t�Y����߿���_~����V�#�����/At��.�eA��~��x�������_�����1�?��������_����G]�\̿������o�~��۷��ͷ����~�����/�p�o�m)n��������1������_�8<޾�������&��?�����q�����/?�C�_n�8p\�_������;�Oʢ�7�_|�9xM����í+���Q_D���V��H�'(?=x�u��������oX�*��NG���o���̚cѕ�"��
���{#V��ן~f��$�u�U3q��|���*��S����	�w������|�ŌU_��1�O"?| E���{���v1ÜhS��(�~2]D�$��x��;�E���ځ��/tHY�*Qtr	:�!pz��8I��[e�K�Td��Dć���>Du�2��e-�5M,+{��?�%���� +d}��N���x	'! 2oy�R.����!>��\�$�h�G�wʮ��\d��
��� ������ʶ������"�J2a���tvg`������+&�ג�h�-Y��H�����<���K+�7�~N4�0��R�%϶8�^ʏ���:�����r�-]gL���o�8�7�P�2��:�?gb�+�?�%L����.��������͋�7AW����ׄ����o��������TGѶt�@�Ŋ�	�ZW<�u%A]��x�>��8^�LPY˴��,�8���F��L',����dQ]�ݲ|6��%�NpU}����c]eǲ.3��.���t��nɺ83�xR�_EK�l2ڌ!���⤇"u)��ʊ�U�����8=R��eɴtJ΂�t1Bю'c�,s�JA��ND�h��/���;bF�J��my(ƣ�*�N�KT:Iz������/�����*�6�h��S�Gk�\�<"��[�y�,�+���OY}(i�L�$���^_�� ����*�k�����J:�������_ɶ�B�A:��d6W�-H4+�����g��Ju�4�	:��R�G��_H��Z��,��T��W�NB�d7�I� �~����ΰ\�	���bN1��C7d�tG���cFJ;���?^@�r�t����
��,��v���	���-ky��%VL�����/�X�C6ͣ�5���H��n8@�tW�p�.ǒ�)�+��"�8N��_?D�'���i{�����5�ni��`Į]��Uߕ�;���h�3�9��׿'�x4�����y��B�0�](�$��a�#u~��kq ��� �฾�C�DѕG��D�����@�)M5H@}��i�Ut �.
���z¨�+Ȣ<��,�nj\n�c�hä$��тsy��\+\r="��.�2=bv��sve�$�j���F8�c�����?�<��LW��ټ&��G���F��a���-7�%t�J�S�P��I�O�<:�p!�i�� η�+��_�v<}��o����.��/~�d�F�k���c;3�?G����ō�ڹ~JSk�ɂ�w��s	x'�_̅��MHg��;��:}=5�qh�A0��%��<�A���w����N��B<�i��I��O��Y�!S�$�#�r"�PfU�4�;��v�����o�� v���u|:�P�l����`��*��� !U=��Hw�>ָ�? �dȖ��B�$�G�j��V���N���*=�R�� ����^3f�5����.���"�ƽr��J2��p^�����#i9������?�G�����9����	#��E7O��t���<5�m�;|Ҋn�j�����뎮[RË��"ML��$�?xc^�נU_���?���!�)g�R}�Ճ)�C��Vd�ߥ�7�)g2��71j�Xa��X���a	�7�Ŵ1Rw����lv�E6xS*�BL�!�4�&(�N4],�;�}Z�4��Π?�������z�a��w����Kj��W�J�\ ?T<�����L|�g��8j4ҋdԌ�2;uLޘV6��,9`E����7��2�;>�C�������M��Kv�L�o�4�nBKZZ4�]����~���Al�$�p�K=�K<�����S��l�b�9�(M��xdy���U��C�#�b��c�_���U�>�T�d���]��P��邮�Y�U6����.P`2��W�v��^T8�G{����i3p#���K�+tb<�8�GzM�˄�\�5���F���0慎`��R(CFk-�"X�L�+����hU�?&�t�V7Q����@�m�ψHr�)Ù���iS|��@�H���%����L%�j���[vSDת1����0�r�x�g3���]���w�R�|o6Ǻ��|�{��׮������	�Ώ�0=���٫u����yݪ�lg��rD��푬58�oGDm&|�Yf�U<v�v�]ލPd5�����L�R�&��8���py���d`�x�"��w'�+��=�I����9��N�fs��`GY�Wdũ+F4��phA6������� <�I�1u,��\|�8���"%%@k��(5�+�0$�t@SA�7����P�}5�c���q�%�k�C����c��xwRvLot��
���t�p@+�0�
�A��2�$R݃��)c��β`E&Ay�Ḡ�K4n���R&[HG����
.$>���n!��}ez�y�&�?v} B^x慿�t"J�gYʻ<,�
hu�v����uӑYt�c"���"����u�b��	\�������w��`y�p�/�&�6��֫����Bf���x&�"k:.��kˮf/M?"r�%N�j���r �&�2[�r�¢��#��Q�Z�_w\�����&�/��i$,ԬQ>�K6��d�{I4HR޹<Ht鉥 c�Y�g+Dt�<u7:_;��#��x���9V�nJM}��)�U;��%Avif�|G�nBG�K�ڕ,����0Ћl1��.Ԡ���~�O�&��df�m#��ڵ�uD����g�dz+���#�=�Q������	��H� ˼�\f:����������h�����n|�Q�~�]�C)K\�<���C�D���%#�M�k���t�|(�)�)Q�SNH���JD�p-$-������OGr���dz}�I6�&q|��a���Lw� nS�n�����F�
և[vڢ�E���L��x���#����d;��߯���;����e:~B��39S]z�� ��>r��ޝ��d$1��x{��Y�ɶ!sxD,h�y�f)�<#Z.�+�D:}��5�Ǧ}NJ�#��8�/�B����<~��U1����[ڇ��"e���C�Ǳ�?���\J��3��G����;�� �#;���R�0��cܨ����˂ ]H'�'u ��/��*�Y">!�#b�5L*�4*��2b!]�,���V�sB$�C���hj?��V#ޏ�.#�H�N�UZo!�7�@�)n�w^U��a�!����5:1E�-S�����
��*�6H�"�	�\-�޿g�J�{���]� A2a"���+��ΝoPiK�!")���>��3����M�Ł$s�=`[3�l}��bjVD���!�����~�7V��0�0"i҉(�B�p?\��A:��G�!��2�{4�}�&�*�eb�=m�'h�Va�w)��`�ָ~�LI��3�Zdzw�E���<\����4��$z�ٝ�u��AVz4:u~�	d%}lwT<�4םh���~��_�,Sw�w�*�I6t�V(� ��T(�G�]��(�ι~��*RUc�d��q��h�\7���	Y\m]�P.Q�8��H�W�bU_���)�;mJ�*������!��6�%�;�~w�W��9����)�S���>̹|�B�U�a*z��qh&�$�s�w�;���v:rϐ2ۋo 
hS$���d���<�f@��Hi��;���ʹ�IR�R;�P)�|Z����؁��*�'��ui���u�    ��1����]
эHg�����!D	������TA������G.�[��(U��|d��O-��������������i��j/��"�o�����Ŗ`y�S�o���߿~�;�� 3VΝ�X"�A</����%Jَ����9p���Cƕ�֊{'�""�6����H3֙��������Bj+P�ٓ� ��?��,�2�h�e�8�T���vSh�J>�?�jŤ�|�}�N>&z�����Rw	���[�O�r�d��{���M��b�7�>=7J��>�h(������A:2�$j���ʘ�C6%���sI�ˤ=.�щ�WѶcޥ�vI|��@�)���	R�c����I��RQ���9��x�9����x��%ն?��>r!=���T��3vS��Y�!�*$��#�e���Y����� ����3��H�^�U ^�<��hE}��ۻ(�C6St�9+O�\�)bZe?
zC��7��ۜ,�b�Qf|BR�co2s���#o�ξH^��V�Ph�'Zw��=l\Ke��&C��b[!b���#�����+��T�얝D�:�=$��=�I�d�ƥ���슄y�;!���d�J�HuD�����b@���.�ݦ�`��M6H��G��Е�!�Q0�a�"o�H��Z=����0���l��\��lr)mN�/m��ŧ� $u,HU���~_Bl���'���[�I���m�B/�: �aH��������~�Cp�ivt�7a�P0\yy�������IL���y�H�\�R��̗�v�p(�3v)I��2��N|�˂��F�_�t�;���"�y@���;Ѥ�I��pF̲nk�@C�~�� #����9�x���K����]=�O���|R��<���Z�Y}�@<���~4����\���<)eH���@̄nB?1�?[U3�����J�?ttހ��v�#2`�]�X���S!Y�Jn���-81ۂ��f-c�C�#䵣�zlE������@H�OS�Ê	n�	o�B���bG��Ė	?��@��-��@�N���M�ڀ�>����>�2�s]rET�Ey���cZ��?��T�Y�:.���Mn�#*+1%3�ٿ��a����MCR a:�e�}p��f���	��ȝAO��lL�
�e��9j�8��VT�OB	2�U���y"�NdK�`p;���)W���/��M"S� .��:��d	�#&k��Mh�D�4�R������1��р��P��kH��(%��A�p��a�*fb�ȧp���o[@,FG_��d��y��A������"W�p��D�jML���/�JT_>�Rl5�N�c��k�$�{�l�?.�ǼADW ݀C�p2`�1��݅����d�cU�J�Y!y�o+�rYD>!-�E��ʢ���5
b̠\�qڿ�\$WQ��ԕǶ���׉I4%61	�c"/WJ�/9�ȼ��%SN��!��{Hayd���~��~dlK8]~�9�T`���:�G�G�rU ��f��p�uhfC�N����.z�*��r�1ޖ�d�	W0�]ԡ��
t�	� �$��492��0(�"����*�Hb��]��g�9V��9�81s�T�[��+=RL���3 =D�]ؒU�lF<���oɈwΪ3S�'�ǱO�F�X@$YӜ�@�9�^y���t�$�=y4�G�xy�e׌��
�M�H~�5J��x����1H!U�dI:b��q�TI%u]-p��BF�c�w�K��|ć�t��.�풺#�D��+[���Vx)YR��R&?�ݟ��{1����or�A���V�ǋ'�Í�)���"�j��v�zN�!���e͖���9���
��ڮ��2�nr���B�bB�5R}���pٺg��W �e��z^��up����~�۷_fh΄6�ɺ��������z�G��.Q��	dI��X�g� w��fr��G������o�1z���UOp�s��v���%�"��JD�q����P���5-	Y_��S`7�����J����Nŉ��J&~hnd��m��Kє?d?,p}���gK���V	�}�F=I�H_�$�]o��	���B��T�M���U47�˦Et���yy��=�����O֔�t�z�
"/�x	j\�!�biT̿�j1ʷ�.۪��������G֙��(�mu)˼x\~8_���3�\�X��b?PI�7U1��V-�vz�����D� �v�m�����e@�����+8��W�Fw�y?�{{;�T�����<��~��r�}��|p|���)_� ����li���%�ՀkV��O�
q�2^	��9��d�tHS�)"��4Ѯ�O�:��J���^U��w,����W�,�L4�<5�L|����|�z�ƣ�F��Z!4P��"/Ejh�j�s]�v2!:��=GTN�!?g�TE)�l�k�ù�A���^�����?���4Mt��t���ok��1ճl��(NtdϏI���6��s��?�yčO��K�;NG;T��i�ހ��S	��?3�ԇ-(�F���I�O��՝*T�56��Wv�J��ً>m�e�f8��Y��	()}�?�`�>��%/k}���6�8�B ����t���u��.]qE1��@$��K±�|��G�ms꘮��B��j���_ڰ`���9�恾�m����������Ͽ��B�/n
!o[-ܿ��l�ՋH�-�_M��M�p�?x��:�~���K��9�4����9�
���_>^����8^�wPU1���\x{�r�|����UFӏdl�ƚ`��A�Q<�����vs8>2um'>^�� B�j-�c�3a�(j7jdDn��"���x��}�� A��2��+(U~h���7 T�I�p�5�-cԉ	Q`�g���0D\��̂�jU��2�S�"�<:��S�П�is=�?�_�����:�a{4b
�:A靬�ڏJ�D&�8��8!b�&� ��R�?��{�����2����[|q���/�U���>h���c���
}x?!�y�䣑�ˬ��h������l߉q��э�M��ߧm�v�e�TJo����`����`�K��O|�g�(:Dㄺ6>��l�\t�y�"�d�G���+��΋j�:��T��->}�`h.���\ic��A5D��e�S}cj�З��/����b>:�z�����́uP��QGq8��O���-g9��.�bL')���]�m���|��˾h̢��������5��3<Bm�RW�qd�y�.O|��/;�OR�5�rw�7�"_���h��|��/��_�0����6�=oZ�'���м/L�.�pl���;S-MZ2�Q�ZL�`ω�6+%SB�5��5ZB��6�vLW˝t/ٮ�H�~iV:��}��U���?v�����#���t��`�^W�'����@W�[�����4V�0C2{���7ҥ<�A'<��|ܪto�ڑ8g�s��+a5eUA]�XTA��C�l)�S�U�M����9T���z���E�j�8�C��/���AD�j4e�*넯ގX�c
]x�Hc�g[�-W��u=�!Pl���
C���[�@�g���r`&��#����.3�����NI�v�S�SDF��&�Y�7���n��L�H\���:����K��H�H���1Y���)��L5�1"H�C��\H��D�E3��Fxs
�r��W�!x���ٍ]�F����ܨ1�V�P��Ȓ��ís�󏓕�uYƋG���D��σz��y�����Ӽ�9���Y���;��-[<w�t�`u&�.[S��C�%�m._2bi��f��oF���Ϸ�8i
ݪun�%�m���Ъ�Υ;u�sf�jز�`&�+~���8�]�ۻΦH{�7��|����DF__j\m��<���R|t�

���H��c����/:�ҭou5�8��=43%�p?�fj=�@}3(�ayt��82^bM���SDQ.x���&��{kl�6��U�kv|£'T��q�	�a��"Ri���E�SsG�������� ��f��N:    �␝?L�����~��M�����(1@�͊X��lF1�d<�3|h��4���0T/s�$JF�H��7�eơ�l'�x;�`�m�S}z,6��$���"�e�.9� k/r��z�X�/%�h�`��-Z9�2�s`1���I����g��v����7�P|3$��ҭ�����X6ϟIU��U�X&e}��߾�H�ᶜ� �0VX��-e��� N^���rն��ꢂ#��eF�������"��V�]�#�+8�)�����~k]�*V�}|�V��&
�Ʉ�<�\ߺ����D}�ۊ��vۀ�a��k�J�ȳɕ'�q��w��z��"�	�v�~N�nG�-��b2I�j'�����+�̃�15�!D1G�O��zR57�C�U���s��K�-��Ώ:����Cy΀�5���@b������!*�T�S�h�����P��G�!i�����5���>rX ���}�T�PNպP�[�~G$�����J�&����1\`����s�(8:_J�b��t����]�3��q!���!�d��8-�.K�+d/Oerl-n�E�Ňe���?ق7��E{\%W�J	0^2
�.x�-����M׈�[�@��iB���Y,;�7}G�ga����P���d��`-��h������ax�Q$d���.�f�g�覅HQwǷ��MVc�f��P����.w)����:��=���o��+e��4!"�m�c�U�Z��$�&Y���r�������1`�+��/���"rJ����s���<C}Ӫ�ʮ��tI�5"�Nj�}�@~E>K�W,3
����VL� ���)(���4UY�U}���BT�6�c�8D]�D� �&DI�g�o>g�߮�$#�g�_���)#{ȩxW�*�V �K\����C���:�=��[�+CT�~jr*"e�xȜ�fKrc��$�Y#ˍ�ڍ��I��J���YD
$9ƞD|��Ų#�l2��xe�!Tr�_"�]�]�g����8�Y��"�v�VP�r���&��ͻ�E�}� &��gK��3UrdIU*��LP�ǀƙ����ţ���)�\EgĉMR�k���%g����؀�'5�p�J\t-�n��T��-(�;_��8�ѐq�{�ezJ��lN���0�w���nyPB�n�X�-h�=abA]�`M�j���Dۀg%�}�H�����ܽ@hc���Q�'�z�sYT"����C�1�~��nGb��k�:��8�	�\EB��А��tX����ɭ�!��8j�Y��iV�zV['��d��������rg�������VY�=߃q���D��my��Ir�Fy_T�6�eޜ�*[\��Sǈ�7ͬ��z���7I�~7:s�8�:)"yʘ���v��pL�'тH��+'n`�g�qkɪ�;&q#�)P�'Y��K��>E���|�)�/�G��T�o�NE%ʩ������@9}��d^O��	��流����'w��34��})o�К�Ş��Cڋ^�kh����1�x�y��6*]��.�#�T�{i��T��7���6�6	14#(�F���ͥC�X��)Q�0�;�(e�*\�dA�H����|�"�r뜤.�U�ٵ�I�l���F��l/d�\뼧+����eܺd�"k砸�i=m�e�B���-_�Fq=�|ׅ\�e5ä�[2ȳЭyy_ơa����@�F�+���Rje��IB�/��.:	�^j�>F�b$EW��qG�{V�U1�	2>GȫJ|�%
$%���P֧�����S�j����*�Z7q`Y�+���M���ĥ��oEn�1�/�bgvc ���������R'2@1n��{�}}��c��Nb�!O^Q~Q϶=�W7qJ7q��-f�?���d���k������5��	Dj��e^\)Z�Ɂѐ�F�a��._;��|zGG�fA����{ʦ*ƹ/��jom�"K*ZHʹ�l��I���I��5C�{���{m�t��u�Y���+��+!�a�6�	��w ��7~	Ӊw.�&��>ԌG.$����M@��.��{i_�g�����j�Ob~xIZtm�Q{�.Jעz�J̫��ׂIl�b�+!"#s���SK	/��dY�����-�d�R����t�����d��{��:���~ ɵC�j�z(M���J��j��-��3���������x"ю��i�:=d���Č�<�?�Ͳ(
1Lҏ��|��C�"ˇDc�HG;���S��\�8�Q�em��=g�1�ME:��s�?d�hn�m�u|�|HY����S>;�;DN,���Y�U�J�p�O�:�:�Pޫ;2���|���p\�O����}��,,8'�'��_J���~;���%�]���UBG�b=�=cI�1t��b'&ѷ�D�����Q\���kT�<N�>��x�NB���ڶ�jLe��e�
,I�h��Ԛ�*��RM�9�Q��"Z9�qh���pP�������J&��R@�H�X�����*S�T�� ��I�m����-��*[��6"�� M��?�O�]^7���C��_oZ�rha�4�y+�X����μy�HC�`�0�F. !Q��dXF�N���"8��l)U<������UV7E�*�bL�:e��%ή�L�`�a��߭Fiԅ�&��6=�g~x��V�x3eLr�Tu[Hԉ<k	5�5�{͍�7�M���7�Oo�w�;B�J� �=���A�O��pl����uTeK����c�����X�Z�A�q[7�j�B}�F�n������O����4���?��h��5�mk:�of�B����,�-�����-s�톂�Zq���1��������3��㢄�P�L��#JHU-�J4�br��]}A��8�����Cܱ]%Tg'��%�@�0���%��z^n���,���vT��U.���ţQ�L"�@���s>��S/c�K��X3@�M�jW֊S]�S3Ҙݏ���}v� Q̐����(���� �����L)g�b��Kc�`��L,��$�z��2������G�M6���������yr�^�r�I�xŚ}UM�����mv<T%=�"�u��@�t�����yey׀n������bQs�9��� �]��Y8�:�,>���寿R���pz�w�e��A&4&��q�֔gQ.5%Ql�'���@^�~ڻ��7P���ZUB���Vp4�.��&����Z��4�b�< �������=ܰ
�.+Z ��S�����b�^ߜ���
j�l�m�Y��(���Z��;�Яyr`~>fY��SD��<Uߧ�>�iU<�z=ټ�\Vu�D�k�0m<�9�����@���f�&�w�+�o*M�$
2�5}���Ie�[:m�`��{���-���lBo�Mg�n>+8F�T���!�,u ���<G���͒��?�Pz��!�
���u,e%?�c�(���x��l����8�^��>2����n��ۂ1����T�d��,Q`�O�BU�j�Cqg��.)˓9��&-�;vV@�E�$Ml`J0`c�<+jp�G�ϳ|r���.޻�_U�rO�j��"w����d��L�#��R��<�X�Quz�0�\��O4�O,T��N��#����vMw�?�W[�l
�e�]'���b�>8����!L�����Cwm�������e�hS�r��v�����?!�"�a2C�׍�2�*�1�B��^���͠���(���+��:�_u�->���	8nh�e}h�{*@/�P߭M��ɸ�4:lK��U{C�c��kN�Ŧ�#(�c'�l����(f4�$�n�mduS]9�4�XW
���1�K�r�W�K1w��"�ٴu5��p����|#�=�>k��6���<	�S�:%�AL_@~��ߜጟ�Z�,2]�,^ı-��&4�&ݰ��E���Bb�Z��L�BG;re�H#�H�'<��7��$�s�uLb$��-ϧ;V���������k��4��[{{��ĜS0g4�u�z�;�F^\����Z�t�2� ���V�V�AP�h�D�C	/q�x�1b�	�
LD��,���9T�k���p������    ҏ�θԅ�:9J5�%R�c^Q^&��58�3V��b����{�k�]d�O�\����	���xܦ]m�c�)
9	4V���yt�֛��]7�6'Sg�k&n���C���1�Eܳ�]��6A�z�ܘb'����XPpP.�T0�(O2'%��H��%��G�*R]�3�jM��c���G�[9�W��l�
Iy?�����E!)�=�����_���XK*����E6"���z`�>�14M��I��ч%E&P�(���e�d���,+�yT �پ�%y��f.'��m�A[	Ǜ����7#��ؘ�㬾@�<w�����X<�h��ō<�X��Դ"�����x_�e�|�F=���ĵ��l9u���c&�͐�U���ϔJ �2�a�xt�R줃JѮ����o�~�u��](�(H����!�ɺ��K�`y�eEٍ�2`qb�7�a�?y^�2(��*+0��l�y��N���[�N��T�/���Tj�(T���*ۨ ��{��ƀ���z�x�A�2zFo�x��_����^��{0>9+jU�j�:Bt7�W	-���2/������"NPE��qO���#	L���E��+ПTx������e��g���{�婂���w�B�P�UA�K&ZtZIJ�1^mhLu'��]�3Ɵh�����L��KM��<xZ(�/�kDF�Z_�g��b7�Γ�ER^E6����T�G���|�muJc���X�W����̓2?U��h�:����g��c����-��0�����f�h5�T��ل��"Z^I����&\���=��R��R��
:&<�}��m���E�X����Y�Eu�{��R�H΂��B�V�S���i���ۆ����n?E�YV��xK��3«����Z��E_W;)1�r��&��N=���>��`m�H#�O�rF7�(��@�l�܄���SZ9=��^�RR÷�2I���=�r���+W�ӹ�;4�����Sq���m������]"`\mx��Δki���qX�����b�u�n�9.m���-��mW�^dmd;�pxJ�&x�br��v��YW�^ȡgx�a�k����.���ȋl���a-�`0y�}���P�c�b���Z�'�����g>�j.TU=M%bdDd��W"���KlP@h-��A=��c�U{�����$�b�O�����Z���k��Ǣ�g�8�	�v����:Jo
�
S%���C�ō���SY�d���8��ꏺ��b)�pg�<��X^Wh�b`0<19U�yZA��-oކ�{[z��o��*�n忐I���݊U�<����'\%��>s�"�f�6��ޡ�������b���3B��oO���H_	������$���5XP�8�_AJA{��pE�!1��^`k�𵦅aD��J��YW��{�$Ç2�ꜳ�N�js��t�Ru�#,19�f��֫
�}C��	���q���׽+�U�� ��d�$��V��	E�"Ux�ͫ�;���Kq�΀z�\/êIk��1.%�܍XL�Q~�Q�	��?����O\��)��@�:2A����u_����.[L�����A�M`t!�j�w�~%n�J��G��A�����f��@,���M�*7
��L�ݽ|\	�c�������qQ���������K�*��P������޸R]��G�ĥ*O��{��$錒�y�
Z/E�G�R�X�5ɶ���h��9��ʧ"��8M�O��s�,��yn�D;q:õ����Ζ�j,觕������=���*��+�����R��<��7٬��,`<ɝ�n0�U5�$�I�����/M��R�2�Se�C���维�p�[.��9�V�D�=�v,/����;�oc+����L�>�i��4*tR�U��h+�V)L5�P۝��y�V�j�-L��B�d��|u��=Ψ�-�D���Q�Ⱦ�];b�/`=�&B�n�쬮���`6���:z�<����]��y�t>X�kP0�h�-<�ęL�z�=���_���������&��d��Ǒ���T8�FhH�B�@�)'3VnyϚ���*W]�<$\�@y}d�{�)kZ�?��v!y��	���0k�>� �����J��L��l�8#��}6�)�@L7��Ff��}���]����n^�3L,"�!Hm+�z���|Wyk����-b�X���,U){e�>!���Y�g�og�{ {v�������jjtk��� "p#��}�����7�!.R,u L�5g���V7�f���Kk�XF��x����w+b.,2e������1��oz�-���N[����z�ѭJvO��A�ކ�VpI���"w9���n����@eŭ�DV�X\�XL)E���n�E��\6`�����`�4�q�0�8�����dJZLU�6z�ݗ,���tV���z����8�З�Cӓ�l�@�-(듉�e^_�������Р�"�뭭�j��i�zH�^�1�)�.B���� AǴ�v�!���6��<��g.�	q��7 Vg����+;�]��=r��mH�����ϑ����<S��	�?ܸ��2X�%SQ_ճ"��R��9^�q֐�c�*���>}���Kt��7�>x�������
�=/���8���O��kC!"����ſee�,��3<m[@ΞRJ���4�u�9��Iw�Zͥp=o�\�&[��#)��т+d�>^�䨊�� /�K�"���u�tC�"[�t�� 1�r���$d�O�lʹu�41]�o����L6l�by��Í{+�W�k�>�j7��x/f��,�R���E�TP-u�Cu���|�3W1����kT��$���|H�sb.\��7����j����Y�̓��Z^�Ef�/�&���C}mj�q/�$�����M)8��im����
.��/4x`"��#I��f���P�$�۩Pǟ�,��L�_<���oa(v�o*�jQ��K��]�R\�;B�;|�д�X�Bw;h	��L�;�j�Z���g8��(��$��n�0a��U��g�cv�6�G�m�LS��^��}�#&�P��qe���R	Y����Z#r�М�F�R��l�#��(�]�^��U�����o�w��pN���Q_LA�Ւ(!k��PZ����א�>6:t�e�$���>`|'y}�xta�:o�85⒕K��V+0�u��<���P
�5)���$���O��U7�J��jć~ïO�'n�H+<5��97�W��:m� T��=�^��`{ƫ�?���J��[���F'-����nR��dz ���!�;�އ����AY�+6>��j��DUJs�F��>���^Ÿpӭ9����f�L�Y����ٵj��q�j�#�:�5�}1��
)�l��,�G�`<��O�mߜc����Y=$Sm�W��~��T�
:P!_\�#kfAd�,�V�h�yYM���nV=�r0���x���"=Ex��p��G���*ǅ��L�)��@���=,�~�����M��Y�N���k ��g-e�gC���[!���S�u&@���. �%�"��".��5ɒ��(�Gub�\�<m����@bA���/0��s�3xq���������2�x�gNz,��}v�i}BP�ԟ�s���$�~sИ����3M�����T�kKl��0/��Ţ��yub�N��7�����)I���/�KuL�������/x퓣�B�>\a�Xlŝ�v%�����	A���ȴq�W:���,��L�؇0=�����)hQ+����L�ʽf��#�5��cs&�'yZ�&��̼pؐ���H���B�����G�u�l�������q��K��NG�����&�f�w������!�eh���C*�6���l�f�8�D�h�7�俎Nr<ӌ��S_��#������>^���d��pc��n7k!�s��ӻjE���\�\<�����`5ŉ�>f	��zG�l䈈�c&D�К��{�� ��DyU�0�vw��A�4�\��
��؆=�������ơ�ϐ��l�����8�B|�r�}    ������������[/M�4��sY.d�����/�K�'j��f\�6+e�_Q�lc�<���L�&@K�h�	j������Eݻ��{�#�Eɛ������r�m��`b���+��-,g��k��V3�Ь�Pۉ['z�"
.��b�i�f	�k�0�p_����<?��(����h*�P˥Q��?��9�2��*�#D������	Ϸ?9��ů7E�';��	�Ӿ�2�{tV�`@�dW�ڬ1$�j�<N� 5�2�}��E��IСk��u0^�,NNK9D����e�|(���gJ$�M�7��4<(?�?_��z����:T2z	��u�,dxU�
�3&a��_�8rc�f�U�R�	]<�vY)s��'2S'ǅV�d�Y�X���*��90V&v��ȸcD�Z�l�h�̖|�l�"�E�|Q��&�Z��PN!wr}IC¨5F\���OB���L���T����&�Z� \\�`$�g���O\���θ8�ȸ��H�e�U����b��y�*?��-����b*�m�FD3��������n)[\�Ѝ�sݑA�.��5Rt����*듡���S���L4�^��Z�[/�h�5���{2�7�*<�m�p�;t�E�p���c��t����"k�<�9\���(�儣h;y�
�횡���ď�- g:���z��q41"o���<I:���S����p��N�]^,�k�ӗo^<.�3~���o$�󃂂�h
�M�;�{����V���́�w7�M'�E��t�^C�`r	�P~�V���ƍ�-�[\Ny�Z�\u������������
ă�XK/�3]��:�oe%��%�[�hˤ��M�-0kE����ݤ'Ru�&`�n��eΤ�u@��k!��EӥkM�F�"�n�5���(t�ֳ��	�p<2l�@lP��La{w"VH0[�{-X���?���H���'���V贺a3�}�t�>껔���[�t B5�f�h�*�W8��K��;�<V�]��U41����d�N½�mN5��[�s��r8?�(rU�A�v�V�ti��maEs��S�,&�؄��L`�7c&;S�I4����r��D��feM\k�Uވ.��*�wmQ#�T�V�ݖ���`<5�Bb�*�S�N+�\�i}�Bn772zrn��u�����tE8^��a�+Tc�K�A���_Uyh��Y\A3���r��	Q�������*�¥U�@�)A"�D�tB����P).q/���o�No5���������{�<�x�fI�¼��������~�Ъ�#����ȣ�3b^��B�JNx�B5��m����ɐOw��� 6~2_?��a"���jv �x�cf-�Eg
��(�3d�Eo197uK��T�Wqq�2y�l[����kn˾:��$:�g�.7V�Lck`�;��a�~qj볙dʣ���4%����'�G@IJ�z>께��(]�qۓ@X���d�\Q��Oq��3ʓɆ,Qz��ۛ7i��ļ[��)3a��	�_:�oD�T���h�t~��.ڒ�=:F#A�|Ǫ�M�1hTH�:,�3��b/�R�\3z3�V��t�G���Wf�G�^�}G���m��ަ�ľ;�]�e6��[ݬX�w�-&�[m��h+��;���B3���Px�t����.�z�=\L޽\�ō�y��^����8ߤ�5[�fs�sd*TIʖN�\b�Ըv~��w���@��e�5�MO2�!������"�~�������xl� -จ��7�A��!t���v\>�F�?S�Z�1ٚ��K��%l)r���F6��F�e^�r$����>t'�#'�[�pL��#X�Z���X�F��g���@����
�g|8����<;��Q�?̸�U3T9�>k~ �x�{B�y�ltG�j����M�Q4�	Y�����F��U�t���͍�a#&k���x��:ܟ.��*���!Ⱥ9ͮ�$�pB�zV�~��Y�+8��:���ʏЇD���u��j$��l����l�[v�
�J�&X�9�.�YUWMY=թ�GO�OG�;� \�A
N��j\ܧ�Y�x��N-��5ٹ�WS?M��ΈK&ѩ�
�V����v�(Ɖ�]� ��Y��p
�do�<ʂ�,�6̐��]<o?�~7e]^��Ϸ�Z_0���ˬb�Ʒ��<]ty7�.�2SA���ZE8&�Yk���X��l�'s��8ӈXQĿ��g����O�A3 �]��+�8β�eG�U�2�u��f���Z���?��x�3�F0֎ߨHX��`�)��)2S��N��xT@OM�?O߉t�M\1������31:��1;"�Hp{٧���5U���h����*Wjt6W�m��+$�z�C�|���s�}�L����Z�Ihd��i�G���}a(^�הl[� ���!/HUvma����<�^?��:��&E'�yo��@�p��m�`�gnl�B���^�C�"�L����^��!��x�3�ޗ3< �;�r]wx_Ƹ�Vc^�/��LCwd�+�4�r�}b��}��0
77�u���$q<�X��5#@�0k����M�U�[7:��@�"<�$��
�RJ���{ eN_�j��b���\���3�w��T(U==K��{A<����cE��A�������V4� �����+F��Y*[, ]�^2��4�H���?]ۉ�$��Kl�s���Q�Յ��T�K=��\5��^�Y���琼i��U�8��Ǳ⬦\x���.��IE�;Vٙkf�U
��
��zܯ�5��۰x�Ȋ&�+��u�<���ܝl��B3a���L��.@@���ӕ��s�Y�����U�ݒg�.P����';��c-h��L�� �L�0y� �|rX��d����W٪�Nv1�ۊ��FM��Jd!"�Q�B�Q���޽Go��Q"�h��|��Z�g\�r"cIj�g��]$���@m@(��F�0�|��-#�g�:���#��rȫvh�a�Y��AÈOXE�8�}bh��/IxcB=m�3��M�L_�*t�u��lQ��:�N���"g/��}i#K�,�~�:e���A7�^�3LW��!��}B�0C���/^:h3vW�>rd��Н����;��![��f�s����O�y�Q�>d�#��<�%�������϶��W����f�E���-�0�*����b]zU�0��,LV�O�'���OĊ(�0��'�s�DZ�yUBHi1�l$ӐO��,�~"��]���s�o��KQ�D_s�'�/Te!�*� ��#^e&��}Bƨ���O�<�����V��t�]���k��FZ�aܫ�Vv�e���Q��6��FQ����4�9O���Y��Cm��Æ�y����Dͳc�8Q��3L�*�p2�HS2�N#�Q��_�61�r����XX7oy�zπ��M����b=���Ez���e���z����b���.}|�W�����[b��HT�I�����@�xd<�}��T�S$ۛ���^sU�}��XR�$9��tM���A���r��R�}�ɤs�{4���k�����VE��z��]x���b�zF�2�_ܚ�[�����7l�ez2k����J ���?����H*��׺.ؚ�;G�+P��3��-�V�z��_^� ��X,&A�*�J̡�a.�ۺ�󬵬=EPW���v���H�|�(ݒ�}�O"��҅�`ЛQ���1�1@�̓>a6�[?��u��f�l��0,HVԯ�\z3�U�"n�<xG�`��A��-2T���D��n�������T]#��E���d��l�#X�h���n��o�k �z�T�8#d��ҕ/8��l��(� ��2�.� ��-�>!]�p��?�/�������3
�1��t������fԫ�e4�/"�̃>!]0G�g?.<�������4y)N��}�Ѳ�܌zU��5ޑ-(�y�'d�X^�g?�-�������4��<Jt<�/"�#_�1x��v��i�	Y#�ww
O�_�q����,�F+�^"��#��J�&�ȶ)M�'�F�    7�}9X�0���01W��\5Y���/u[O^�7L��S�r5� @m=��yˬ��	��s<�;n�;X�6�"C��zAx������U].���l�&OK�\U0�ۇ�.�G��S�Q71V?��#�{�H�~���s���#p���5RM��o(WѦ��!8��?g�I��ג�rvLć�M��V�����|d�;x~цꢅWjK)X�U�,�4�#ޕ7h�J���ű���B����i�4Y'�.hh�P�pE�)gw������dcmL�^���I���"硯Q<y�Ε�oT�9��p����ͺ�}�;
G�)�������B�8J��G��"�����Ӂ�����彯��I��ޱ0Q˨O
o��+A9U7��M�=�7�P9e/	�"놁�J���;rV��6��F����?��r���K[9J��𕌠�����^��29̣w��Q���2=�_�D��ۺ�/axL�Ѩ�OD��7�����tCī��e�P1�y]�!��7��5��u=tW�!|��hA��6�*���F�vث�E��e�la�XF}B�{���b������4��F��������@	��#��9Pr��c���%j4��|Iځ�6���)X(��ko�y�e7*��0U�D��ͭ�c�:�U�Æ�����,���O�����y���Zc-�j���P����/�"��WW A���;ed�	�#�v���E�F/o�`�
u�B#�K-���������k'siC�ʕ����Y�X���#�J��žvA���	b-Z[�|LU��8��m�х��W�0���Ȑy����+����ֽ&E�~�zm�� M$�Q�$<��Nb�po���0\UaE׮�F�q��#yd��CE�M�\�I+s�����ʅ(+h�4/	:� cdg6{�	���d�����	.z�]�b�&��}}�����Z��N�V��S��s|� ?�nX�	�o�v���s�����[���ȳ~��c��=�F�j�[X�������U��T�M�)��k �����\5����EP7N|�&���_x�p6�典�;/4�\�C_���������F��,���G�`W�Aj��j }��y�A�%�����8u@Di��B��)��G�/��ix�x6�Ț�Ij��Ҩ�m��i���Ά��}�'�3����)��i���#�YL��]�B�	~&�=�-c_�{�H��O�%�!lo�벏�ޟ��+'b:W�Ӊ}&Fů�c_#�B�~bG��l���i�d"OĨ\��_����,&N�j�?��>���Wc^�[�*jj'Pȶi�K��|mj�`�\C%W�W#�:d� L�$Y����x.S�>5�{U����;�E�m�'�v����2#�C�Ӡ!�|ɖ��?��,\����П�鈌+�d�U�9Y����S�W�d���~=7�t)�ݶ�FjZ�H�����:;��x�Zշz6l���"�I[����V��c���0��,t7�i�a��7a#c4��K��ܱq`V�׺���=�Ȉc�B�r���e]�5%�H�[x��	��ڍ�N���B`8���s�4R��92���en�L#���;]qW�ϳ�I����2����P�9TBF�̖2	W�pY�!�t�-%�<����WQ���bO����c����3��'Yb�Kt3�V��Sh櫜��H�:]K:DE"�4��jE�m|�^���g�x~���}J����{��#�2z��#�p'�����wf��}��8	�Êڶ�9�Z6���A�J�Ʊ;r4������B�7?,�l��R�s�>z���2�s�rȫ_4�ܑ'<���K3[[��R�n�%�O��aw�>�L�:��A/�&�3��K'F��q�'�Bg�����+���ۜ=�����,��L)����c�{4���P�U+'��7e����4���x���5C�0�7�.Ч�D���3$�P��C��ۭ��70��fԫ;!���;[��<�{_o��'�X��cwwq�����A�2q�n��;u�Y<i�)!+bm�\ڑ�6�#���Q��b���_��䏠��3�.D�B�%�?bY�H���.������܏6��
������b�{���He����P���\��!��,����u�<jþ����k���6��E7����wbb����Y���
S��v����G�����Ի��u��v)U�[&�"ҿ����L.-���K��8M��?�M�ҷpK�y��(�b�{b<�.a{���p~��ǌ?����lx!�]gM�[l�����^)Z�q�8�4|N%�5kD�Z<E�b��J���뉭�^_7��o�C�Y9�2���ٙO��%C��ƍ�~B
�.C�jNZ\�81�Ezs���g��I+WQ,? rW�6`*�H'�Rz�ؑw�Cø�N��lk�Gx۵Җw]|�N,oܬ��{F�޻EP��\�ll��>I�/����HQ~�bh��ZƾJ�D��;��{C?A��hO&��N�K������])��ŮS��ꖱ��{�'v䏼��?(���<�?\��_ؗ?|������Y�_�L� (�u��k 2��#;�������X����y�p[c-��M��"R��!���D��H�ѵ�}�1�!�����U��
	\�{C_Z�k��|x2�
�X�g�/X�+�'����_Q*����K��a���*}�����a'�����?�'���������3�U��:a�CG�*w���vĎlr��OHV����S��}���I�jV�J��'5Qb��h�f��fp�K<'G���,�J!%��,yu7!`u�'v�̳����Q���L�ɞ������*F���OsU�Yu\����ɳvP�y׹���hb�p��I4m�va����Q�h&�߳��Y��^≝��v�h��8�N2�þ�j1z�Z�#�&Cm>Q�_ ���|���:�J{3�ű%�Y��M���c�%�Z��.s��XZ�x� ��9��e��\%�-�v��Z��ZHg1�a��đ(c��\�A2���5!bu��J�YK�jRm�:�	�嘙��*tξ���?�(m����(_��0���m4��i�,gsV��S���g��7���T�`#��NF�6�lTϦ����;f�~2;�����>syS����[@���4���n�ᦍ-8�7�^�ƈ�4�!�p��}�#����O������V�1�̯&q�r�����6�i�2F���v�h�u�'d�D��)<�������\9c������ㄵ�o7|U�ڵ������q��7��������0|_�Iz�gqO��sY��`����h�F�șآm�'�Lh���e��@��]	�&�ί����8u���������9u��q�K:%����Od������v1io6��Ex.l76�{U�nb�#j7����=g��ڋ�����a�s�R�习}�<�UI����Q��θO����������<|_�&�Z+م,sQ�~&Jԁo�\Ή�s���(!�m�KN���h糭�M�%�b 6��џ�-kU�o�#(��)qp����aضzm���׮820H^�5����~$��s�֗E��H��Up�YBe��uQ/����q-�G���,U�%*n�4EN���ț�N�ܱ]�|;�wUi��HS�cu�	Ū>��ʋ����J`s��b�0U�fa�+OLQ�L{!�����t�V�x��b0	����8^Dyp����^���$�t�t8ڕ�&u����۴7�gYm��X��xBTsW"N]�M�)��ku� k�3RNFMI��1T�UJ2�|�e��
���	l)i�l�p
zo���4��.*��jq!g?�d�s-�\�E���;��cl�߰�z��l�m?o9e�h����ゎ<sA.��@8�����F�a�Zal�c~��u�'�0���}$�B�`�u�h�.�d�qL)K<��`�E�y�R����;r�ݝq��t���Yǉy���L:�TD���nt��u♆�*�ķ�ޑsXG}B�I���Od�����V��ds((+�]�������    �9��~`GԊ�Z~B�i�d
���5�ߓ9���qX+/���1C�4,1{M�4:����:p�5�zY��+�ޗ?���n�K�Z���ֻSZȞk���A��?�#jp^��OHUv��D������N[םѹ����W��l�#m?����A�w>�����5�ޗ3���7��Mi9�nƽ*� ���Ցc$h<�@t���+��F��3Y���Y�=���##��4�.�|���b�ҫ�����^�Z�qh��L�n�*#�����cg��}�څg��u����ܠu}�G)���Df1�KQ>�uj���2Ob�4��J��B=ˬ�BN�JB{��ux%)EEGl�$b!(�|�8^*�k��S�z��S�6`@C@�-;uX�Xl �sV��&��AW�R����ؕ�M߲����Ou,��u�}��͎7q*İ��a���j��]��Á#��	2�\��{��Y�7!��g%J��U��m�<��V��-47$߮���&M�B����n��9�������� bvfC�3u��|b��,7�k����h)����z]:g8�p`�v8�;0���PL>�S�O�. ;��>c��}�N_�<Ӡ2{���ж�øWm�ȵ߱aJ؆}BΑ���O���a�����u'�X����HQ-f��a�J9��w�C�2�2���/"b�����%w/�g�$�4F��#�T��a�s�J�`��F!�J����a/I9URF ���[Ŝ*1�alm�s��_����]�\�K،zQ�.�y�]�.�̃^��j��}��o���օs׍�!y�g��%\du;�U�xZF����>!_P��?��o�KX�t�q�U �e7�G�X����We�(a��!+�j�	)#�x�㟈Y�����嬼��8�ޝ���k)+�����l�#aEU��>!_[?��F����caR�~0sxe̮j�p)4���k1c��?ΛO��Q�`���u��/��F��䛕BKc"��4���ݷ�Y�`��%&�ƙ*+����������̽E�T｀7�D�}Q���S��J` d�Y^�>�BʈF�ϓ�Et�񞵽�/Uv�D�X|uU`�s�����r����(۱����>S	�ş��la�|��H�X60
',�/b�w C#Yv�� �I���.Q�F�t�r�Kl��}�G���t8��҃Z��b&p]����k �O=�u��={]MWQC��B9���n�B��um�����t�V+�fK�S�M}C���.�b����X�Y�w��r9�l���n�����hF=xʈ�,q�*T�k�}�_:Ð�xżQ)g�A�Z7�g�cܨ�[ӘO�6`�}��>���}�¿KV��G�k��j���oF�*W���4�<��E�����~���}�­�ys}"�._�wG�*cX�ؑ3���?!k�Sx"o8����e/��y��{.s�Ƒ�����vd������9L��)<�9\���2�����1be��I�
���ʛ�؇��M�B��^�5�L���v?�&iM���?6��˅�A�p8A��Bٽ�aulF�*c��Hd�<��E�����H����}���K�e�M���z�"�`=�Uɂ]����8�rE��훟�����}��׻��*Wl��*Y�i����\����tA��_�/_70y�9��J\�5W,9��FF�C�-�҃/Q[��ؐ\��yU�&Tew�k�e�<moJt�A,���ނS�Į�*9�=��ϳF䈓,�H�~%��챸	8	��6����q�`���a�@���гF���rO����8`�f��T���)�=_8�>0sJ��Z��Y;��pXS��w^�N�-�9M����ݸ�슲�t �	&b�����KT�CcC��[���H*��s��@�3Z�#\���L�8��l3d�W�fy�KYM��{qü8D{���%���0�e�V�D�.�f��4�����žUT#�`x{���ۻ���7C���o䄄朐W)�"x��;�e��>A�����'��r7���e������=�=C\�zU�T5��"��A��0�]���D��۸�/[�	��GW�,w��{ق&nF�*[D����D�<��E������^���}��H���C�D�2��A�J��8vG�(\`�	��Zھy_�(س�+Ud���w�����?w�B�H 3�{.[�E2�u�Q�.ċ!밗$�B�H��x��]%dx�-rv����#�3��߾/X��s;�U)#.�2zGȠ��Q��1��˟����}	��㘳,v��z�}nF�*_����<��ET����n���}��H�=j��yA�w��M.+�n��ɯ���ܲ\�\ZX:�����eu�δX#��Գ���1�e�>�y
K\���X+��2+Tg�j�/�~dI��9;g�'�V��k*�[�/#�j��!j�T����yU6X��]��I���m��l�"r2DQR�M��F��?�fk�MWLG71Qv���y>U������<���(����:��<��A�Q���i�O]Һ@�*�s< ��7>h:����C��N#���LJZR�h�Pq>����K�ѝ氊��&j��wz��L�Q�x�6pI��o���	z�N�R��nK�8�y�+�ֻD��B�YE��rP5����g50Qg�"�W��3d�&P��|�~#Z�<x�~�#�}�~#����O�7�������Fb��t��Ku�mT@�{U�`���;�E>�e�'��i��'��r;xW�N���3$ԅ��)�b�f^�����n��>�{<�=HCR 2��یv	&g򛞾�����~��h{�%�q#k���)���♼���%�(R!�.�w����xl������@ʕ3a���T	`e"�9�;x?5����^~*~�����������y��o�VZ�+�*��ŴW��~*vƱ�`�ۻH�����$6��	���o�;���[G��Q�J�fv&Vb{QFҫE�2;f�0F���7 ���J؍0������헳@G�0�+��#��"mG8d��,��y��/w#rz%�D8D9<��E���V}gIUjg۸��5�?�4B�3����؅��h�1~�H��D�y&g���"HL�Q�uU�j��F�g2{q��au �J&����a��4V^H��DT8�;s$��l��o��ً'rVM�<A�L"/���W�o�	?a!��3@Dv��e^���j���Y�)D.���2��N�E���h-�e	�q��eC���^K>,�∧��:>'q$.��(��%�4��v �|��cɴR�8��"��I�W8�~��9�-GA�,G$���N���0�&��"�V�1/���8��8�U�	��Bu��$�n�]��I�,,8�m�q�w@F)��g}/.Wv��^ �V��RLF���V~�_�ng&?p�dЖ.Y��6ԧ:���(���U�g~�?��yhdƖ�h� �ǚ��c}m�Y\�/9� ���v�!���璗����g����p�DOꂺ,f
y=�kj)l8W��X+l�V�^�����`�`m���O�M�F�fy���a��7-��I����jv�=�Ψ���s��h��$`"�i�xK�;��n@��\Ѝ$�o�(>��W~%��_�(ȠQr׀q����ހ�E�.��QF��sV��<���@c�`9q��V�F9�04��k��h#��"���"��HW���歅���=�~��ڵ�ԏ�@���e�H��ZOs�X_�Xc�L�.7�y�E`���&!��&n����&�߈"d�:>�mEG-��N3�8 �N�DU�2��:~k���`�݋7�x�O80ǣ|���3�wd{�>f�_(Jw��BTH5�텏	�E�.�lb/�������q8s-�6WD%�5���ߥ?��0�3�Zl�QF�E�r� ��[�va2�Hŵ��P��pMWEzΪg���+W'��k�}U���a��Dk    �C����q/���t8]Wd_��d�R�ߠ��f#g�&��S]�����6��<�������L�vQ��MmN�h�B.YAk`B��9���R�|ű���'C��60_�w�W�Y~����G_��#/9�f�4���FL����%�q���\c��HU�'�1�(����E*3�x�d'�R�r���]�tP�vN,[t�R�+9Bi"�j��b2^ �gMf�*�rS\�m��ڡ�90�fє��lv�FFF%s/s-t�ܖ�h F�s��BtCMt���^�SU�S�~A��.(�An/�E��M��uA�lb/�]$�:>��v#�?�n��$�;���r�S�0\���ɪ&Q���&�pE���70Et!����(���ف'�d.d��	�iu�	�hyOdEZ>xO8�n<��5�;� \y�E��	i�t��G���}4��17ڮ:�cQ�e~w�:1>1X�Ip'J1r��v�b0G��~�b0A��o����I�i�1�m���{�ьc���&��h#Q�*�@��*���|t}�����h#@G�aY=E���U�:��eU`�@���+M�^|�����y�x^�2i5Ni�S
�j�5��&�.-l�Ȉ'�C�sI�Fz9��v��[s37����)�&Q�xPeyO��[>؍g��M�\��ow�PM�^�����Ǣ��Ku����oZ+s��HL%�3p�.�Q�f����KK�����Q+�b��"��0��S��9�*	oS��H*XRjɜ0T�+�%��2.��H�ۭ�:�Ǿ{t��%Κ͋���h���V�Eހy�2�����Q2�(���2y��s2@ ��7+:/�R4�|	i|Xy���tw��Rf}��d�vg{�����*c�f��d���n�u{_D4񮾠f�|�����/E�����|��q�mdł@����^�P#h�dpR}���#��W�]��o+�@?�a;�[��6�P��A]��(��dT�-x�H���^`�ʹ&7���b���c[}���v<���<�ɚA����t�$��f�f������sI7��'�wI��s�����Jj/�`gfa��g�B/�
�f��d�\ɺ�El���[���(w+H�Rh/�H�4�:��3ʼ�+b���pC�H!9Z�eN��8�fk��� ��"� $�"����/w�^��Z؉p��bt2��u�e|��\
�D7�3�ڱM�B�(��`y�o���ȥ�U�㰟	��ܷpU� /���
Fg�u����F�P��}��.Eݨ"�GD˺@?֯\}��{c�� ��m�؂��č��/�O��v!3� v����3Ȉ�-8�gD��x�|�g�~���!4	�Ew�Vy�x�c�{kܟ���v��;�s�y�,� g�D�J鴋�Ľ�6�F�p�
��)��M;4��u$\*��-F}�֡w٘{P�[Î8g�y�l�c*'.,<���$��9����pT��d�#6ppQ�x��/�E��i-��~j�)��	G��1���3�~8K\�ۗr�(3�o$[����bn=vR�	%�x�L�d,��<�PX<�~g�={n?(6�X�)��Y���)KV-�Hۇ���h�|�� y���=[U4A��E�`�l��R3B��R�d}q]F�X�'�um���FHQ#�ĉe6�;W�v����s��!U*����x���]�-��e0ՋŉX��	��4�d�j����h�7�Fɽ�����H�]����9�W���&y7��LG���N; GYȵ�^��a��P�a�E���hY�|d����s�3�LLM{�3D�!ff��ۋ1�>m����Y�^@)���߀��ki7Έ�&�Q��6j;������v����E�r
�f�ڏq�dT���!N�];NG�mf�U���a\T���"�g�v �7����Ǯ��#����L~wgaj�AWR{��3;�E��,��`~��� 1ٕ�[T�C|���ׄqO�Ilc�0���\�LO�õ�.,����O��0����,&CDՒP��k~���� &����"&�>����ڸ�7i���V՗�3�+���P�2)��6���qM2E!ɉ^�݉�s�F�8{�l/� 4���dC<<^ ��F�(T �̊|�2�����:~��0�%/���F�G'QއFW�x[��A��Q@-�rix���IN�9���Tw?�ɍKg��4�+��&ċ�+�]��2;:R15}vA��fH���{Q���A���O�ڮ�"�8��
��#V�1��t�����Y	_�pZ�<���F�����UA��T���ݗ��B�*V!��7:U�'7Jl�ed�=��Ϣ�;H[��m0��+�k#�޿��%��ɟ��_ޖ-.>Fx�71�Ւ����U��çm ���� y{��/O���\5"ѻ����� ��$�$^p����_��#!ěD�`]}��i�����؀T�D����I�t@�-�B�I��8nw�f}��{ �^
�E��(���(��`~�o� �ߥ�Un	Y=�ߝ�5������"�$���$���z�v�q�w.��3�m��{�т��̘�ev�!��$j�3c.hُg��]�o���lX�j���Ƈ��Bh/@Ȣ5�:b>g�y"��ھy#��.D�F��*�V,����!E=̙�^<� ׂ0A��/ ��[�n��0�L΍!b��{�_��K�Q�=�Bf/��z&Q�x6oyK�=�o�	Ҷ�t�	�E'�>��k{ٱ��\�즆Ɋ�I�mP���r*a�y
se��'.����(����dA�.�8c�(Ĵdv".^���~p@)M��7��>4��p"{�<6!<��6���D���I�V��X�*:��"Nj��R�����Q�U�nsj`�^s~-�<�~��5����DәO�QM�
⤁�4�ţ��)�XW�f]2}��U���2��kͽ��r�3��"Nl��]�{{k�7�_�o�j��49�O�u&����"s�Ԓ�M����cKn)�7�B���!�����m$��W�}$�N�����Rf��.
Z�/MNu�o ^w-�lQޙ��`�$HB���m99�g+�(*�����vP�L�%�\�M}��yE˲io�4�V���}��d��7�<I7�G�0�ķs�ܦ�3�����ZЁ$���/�6h��[�ɹ1Dh0�8|��� �4�E����S��F�E��8"c���@"�8t#��`��������;2{��4�:��c~��h�bZ>�	gt@8q!�0�����&�$�-$�a�*ׂV$I�3	�Ƒ~��|��:���"f�&��Sv���p�[I����fa�`sf�0E����7p���R֍-�;��w?w�+.pg{1e�t��Dn%���/5~��{�ʹ1��a6xV��$SFU3r�x��'S@��ՔQ��o��nO�~�s~���h�Zڂs�q���_�T'g��%��B�&j�Y�L)�G1�@��s��*��mpW�����-�yx"��\��*�˜��Pb���ђg�V<kZ�k1����Kم#��t�����ULD�P$�I&�,����h�T�RO��o�4[1{Q��K��Ϯ�
��D���\�U�GT��Tf����d�zW�s���K�&b9�觛M
]�B� �UN\gF&}�yl2d�FUi��|1�����HMl��P�R�Č��d�39� �2�q�x��i���fK&'ԩ�XlrD��5#�³��v��pI�%����Z||�է�M�<��-R9�q>��#Q�Yp�Y
=�#��f>��� W�L��5��8"�o!ܱ1ի$�ȕ���e^�%@Dm}Ñ@,t)��"�$V1�G~u�i7�u%��SC>�Y�,��Y�d�G���-"�+Y7�tf� r�E��]�v-�]$fZ����Z�^�)��/� qӵ�aD?�`"��)ex=� ]����6�ځ����$�U|���nH=O�N<=�@�pr+���    �GiH���b{Q�H;�ų{��؂ۿ|^��7�׵h
��$j<��?��d�gju����Rad��/��Ox�	���c�O�r�^���HX΢ѪU�q/����Zl��"U�"�0Rx$��I��/�0R8(�=F� �Z�#E
�q��)��֏�g���Z^2RD��t�B]w�����^sCJ�Y�ampd�B/�����ak��ֲ{Lw+�,IΦ)�kh���}�>;MF��d���Y2�\��q�7צ�:�k���	��"�0Nx������/�0Ox��=��������L��P��}�.=���ʱ����u�t0dΰ\�OI�TΫ�X_?�i�� ���D'�@|����] m��bÕ�dl�3�i���4 �g�[
�j>���w�}�A�������I�?����?	z�t���&�t��>I`UEcl�"e�:F��.�F���ZkK'Q����P����p�uK�%H���Xy�;�}مYA%*��ql<[_:���}��#�#�������vc:��I�faGĈ������d��[���F���Դ)t�o��]�e�J�
�˕q���1i\�����*�1��Li�&�Kd8�l��}T;1-��&ǯ���^����;�@Ts]��]�L����r��6�o�'*���'�W�/(/b�L��P���>~���Fky�F�`�n#M3�����(?U�m���b{��6L���H�4�I�`�Ķ_�a�Yd�c��D_0R���k��}�?~��fFky�H}7���H���{+��T���XSh��6�&�"���B3X��mB�4�������V��`��d-�f[��������3+Tc�%���z�M؁��ل^��?8>{[?6Ⱥ��1�`���s��a-�����:�|������7`ŗ/Eݨ�d�K|������6̂{�C���0rȽ�r����7��ю0�h؇��:�/x5�Qlۋt�X�8G�U������R��-R+B�+En����E3�
��4JV�=�S��b�>H�xխ(���Q�ϚՂ}��gڽe����[�.j����\g��B1�XP4�7Ƣ��ǃ�3ʬ,����ʕ���(�o8���
��p��#-��Z�!EM�͂i��Hͻ���F;Y{��e;,��`9\�u��7�e��H�r� a�83�82�s��1�x�Q����O�83��=��U�q
&�U�S0�]_�q
&�I؍p��&�R������Zh/�i`�u`����M#�7o��Q���U����đ�t&�j\9αۉ��ZE��&Lozp�m�29�����O�ۨ���ti~�|���à��>��������?��_i�lY�_�Ϟ�1����p �,���p�:5n
8$�v�P�A�l8;�K�L�f
�/�v�O���$k�6����Lk-߼�+�C�nTΠ���A��V"�x%�3W!�}������f�=�ҏ Y?t|�Z����kYg��$SH��=W�� ���h+�J�Z������V���_0ɮh+~�`��̖	X&�A���K��U9[�u�	b-��6��*�0N8����_�a��)���#���|��R�@8LLb{�_a�v fV��oa���IZ�F$��5[_�hA�0���^|c�&�7�mB����y����t���w1N��k��QS?�"��^�H|��#�H�5w����I�+=���\/-��)�8���.t�c��G�U������������s�ҕY;t3��)Z��@�zj�L�&.[��͢��2|�����>���D-3�7pj���hJ�\�����bK�>CimKߡ�G)��]�͜��k������W�������o����jxF�(U��?�rX�Z��_>f)���U��1Pٓi\����l��q��ܿ���u�Y]h���c�<"#E�={~T�G���#=�`V��k]ɺi"B���&Q%���_��I�|{�1^�>��#�I�U't#����n�����8M��zf`{g�9>~g�B�n���ⷦ�����bG�c��be����ֈ�:~����q��܅��q���q�����-�{�9������n�)\�
rltF���#��*� \�*�ޠ��߀���q7�LG�c[�6��#����ˁ]���qË�L<��~Ȥ-�>����_��׿��=��b�,�`�����K��Î8Pi{��8^i��+���Z�mD��F���g�2�]��E�i��z�R/`�O��� \	�F �?�2��um��yk���$e�bO0ʼ�,x��ݸHX1G��uC�[���ql�RJU�|�7��	����Sl
��vé���BZ���J?r$U�@F^��dKv^ T�;�����v�D+�f�H��ypΑ"`P*����|2� i��J�am��V^����0Mk*ɒ`O��S]?�_���ɽ�ڽ��b�v���E4�9\�k��٘j/��L�h퐪G��J�B�@�m]�x����J}�8�OBr���7��N9����w�&~����'f/��yn?�G��#���y���QT\��~w�okWJ���EU��>$ؒ��##�l�w�'��lhdע�fs2����S��5m{W�}�Ec\���h�b��zg�%.�У1��������84���Vy��ʽ@�����w��&q�C�ث�RWd*_vD�Bܧ%���j���A�_@d�9���5ʻ1G�&3E�܅98�Qr/���p`�7�v�0�uast��N�#Dc}����_�;I��H�Qrs�G����s�G �v�]���<�u����#Dt���}�9�~4�]�{�cpP��^��;j�v@vl�zgd�ڿ|d;��n����Q�R�rW��0H�Zl/�H��H;1�H��0�)�_��0�ka7S��(�q^ij��Zl/���i�xOk�zaPk��o ̑Ε�� �Sѳ�_`�>�gQ$| �M'X_Zw���O\���N(3x��cF�M�)e&o�x����ni�L�[1�����;-� �ZTr��ދN,���������x`$��=����W�����Ë-��guG����\�@�6�C4a��E]��t�jF)	xq9�!���F����}���b�w�3��!����y9���F�(�R������8l�GHb03_�Xz}�t��	q��A���e�5��rbњ���P��	�|�_n&J�/��c��W�D)#�x���<�~~�'��t��)��bK�W�z���h+=��Ǝ1ʄ8��y��:#��r��P��Հ.�����Q��hS`
�!)��6xʿ����Ȭ�)dffa�>�5j��d����F}�oWڪv�/��̲{)/�G]?ᠽL �/P_��ndå�0��ܞ#���*�O��d}����2i�v &h�zk$�ڿ|fD6��n�����C��F��U��{q����j�u���6ؤsn�c�5��N�c�$B���X��!�Q��$��4�*o<��w��~�c0I��o����I܍6Dv:���t˂f�}S0� �F�m��t��F���킻qpK���9��Fy���� ����kk��H�B{�F��Q�2X�Q�|�0m߼-��Rԍ*���ѽ��������؂_��_�
i����@�C��0��h���|�8�I]�[��L�,�6<�v���0#��6X�=/r�>��wA�9g�ʭe�m5�8��刡�]���b챞NS��%���1�'��~_��B�(��p��6��8���8hC
���
(�T��ǣ�]�G=���"�}  �ȫ�$RÉN[�)3@�X���I�Y���1��}M`K����?ul����/�H(��#|؇R�=I��δ�5�f~�T�-��fo@��dQ�<	i�����%2�>��.�}�8>ؼ>�tGO�x�!���������x    �b���,���t�]xn�-߆u0{GO4�Q�o��C݊e$By�d,�-q=sV������ے��>�j����mn�$�Ou�#A[�����K���o
��~]�o�V�ɇ$����B1��!`�1�]�hǼ�>,b���#�M~�m/k�B��}�2'R[�ɀ`PT���j�vf6�/Ĳ�?�v���A��Lu�v�����:^`�iG4 ��-R/ �h���9l�v#�a�t����ZU�4r)�_d욅�2�4
��.SC�go���ꥬ[�3gェ�����L� �a��Z�� 'L&mr�qN����~7�	���N���4�����꠱N�'�b;�N��k�v�Bi�ze��ڿ|cĦ��n�U�A�֪s묜�A�Z������� �I��^@齎�߀1i��g����s�c��i�6ڨ�i�ދ9ب�Gȣ�[��AP7��1`[�=^��[���Z����.��"��]��pp[��8��ھy��Vz(����f��o��j%���W�����	��1����g��N>�	>�mȢHN�i�p�a���z�#p�c�4�p�F)����k5��lQ�z�����?ū0�z}����ǔs7,��!ዪf��r����U\h�p�f����M+�w�6�Zং~�,.S̐�j| d���}<x��ogcg���������0z3T����1C���o��"����'Ӵ��������8/Rq�2�,�B(�xM�q�u��s1��L��L˼������J�F��H�����
�1�S��s�n3?��ȌlX�.���!�V�u����v��s:��_;��vU\�{�Mt�ʐ$bU-�;}������N�p�5p���ϗ�Z+N+B��t ,�Q�f_A*�"���$��|��|!u����p�7 ��_��vbLg�$��?w��;|$T��ʽ�'�����E�8˹=�&`�+�������p!+�,�V����� 
!敬�WG����E�|�����(��2�C~��F����y8[�� �9 ��1(�l�<�H;~�2/!�K��r��炼��9IA}-�;�#E���'�s���D��C��퍁�� �H����s�9]
*g,Q�Ϭ�$�{�\���<�c���#}�9��9xm�wc���l&�ѓ=V�d��^�m;ā?
L��_�&�[�٘k�~�=�GG���G���'��,�w��������ǧ�G�>��p#��t��'����H#�n݋<R���@D�!���zbF�(��]ͣ�	-L�]=u=�v�g�[	r5C�����,�\[�L��<�c�t�f�R(Y��(��2Q+A��m�B�Zd�~_��sQH��;�����n�Hㅟ��K�jt;-:Ԏu�v($}l~�ݖ-�t�?��ol'���9��1«�aO����ץJy�`/Ӱ3c���\GU̞Jl݆�'�����F I�����.9ҟg�)��u�;�Iwh�BU�������݃4��|�
�,\p��a�;�j��9I����F�T㾓�6�M�R�u�Hs���cK��i��
k�z�:	���y��U3;��.�M�c1�Vw�#K�L�%��Y�pM��G���A���W�ں.#�xc����	�3�?�Y_�}{��rn���a�m���`�0�]����9��X6ʻ�:D���i�����3�k�C�n�F<b�����F�#59�r���o�p��[�n��0����GL�7	����:G�e�6[�G�lB��1��.�^���~�5���~�p��G,�i�Ex=
'R�mZ/r�-�[W�%��"���S�f���{,���Y����i�.�c�e���n�Xe���83�]CM�#yH{3��h�ݹ�fH�w��ݶ3��.����!�c n����~a�mg�e�����쓹׶qq��>�����m�:��;n��mNO8y��Y{D[6�]��^���h��U����`�x6�E�4��g�_�_�f���M�^[ƅ�k�,ٷ�Kv��5㯸�wbQ2#߽r>�V���FɽV���+Fa#��V��9�+�wm��cŸ�3J�`�x�b���V�W��Q���fH/Y1�M�~�m�|��N�8�mðD��^��E�a�hj{�z9�a���E�)����ӵ�v�A�	�k�|k��}.�-q� ,��� x��[/�Ŋo\���M�^^Ki�E�/���gW/��!_�� ��a�BB�������;J_n�0$�#��,�4��_�;nq���LlQD�g%���w�ȼ%�tp���7�g':dSB�_��T8�3�q���Qty�_p
�+�%_8�������1d���G��1��Ǣ�F���b{�ҶH;���E�����_��2��ka7����2�+�T�[5̈�Zd�b�h��'�#J�}uDk7�=�?,���O:B'+�K���4J���I�8�GH�.������sN�I>�Ȼ0'1<�'8Q��n��1���6�������F�c���|�����+��˝�"$���-�aD��p���(��ǥ�A��^�ű�;@F��&�ʈ�8>~f?�n����^�W�gh3�k��(#�`�v����E��Y�������n��Y�q/C��3�'�ڋ.<a��[8AF����3���O��t��Q^�p�W��㑚�WK=�����㠖����EӴP���+/u,������2P<��1yN��Q��6#�B��0O�2Ɖ��K⋦�\p�<E��"&��Ȝ���M>���٣�P�8�QIA�����_����5s*�̺bs��Uu&F:}�N�`H�,Nk��&ա{�2)Y�����n\��P��+kČ4^���T���v�!�6�Ǹ��T&�3i��t��XC��,��y�@?5,Pƛ03}���|3с�y��9�2c�$�r0�*��0�'b��i�x�e��b÷|����Y�BC*�u� �~��
7�+KQ7oE���@`���:�l1X�.�����F�o�6x(P��7��lW>����n�}Og���4!�K-��X�.D����L2�\��Gd�����M����g��`84��v���l\e�|�&�F��FɽX3m���m�Q���7W�6G����V�?5I���
"��6}�w�����X�(6n�;ƒ�i�]Uw^p���+n+Ft69�b綹:_p�{��B;m��7����C�	��~��@�m��6Ve&V�;��C�y)��D=���f�uz\[>{�az�m�b�b��p���v��K���� �W����$R0-���se�z�,�(ٿ|�0Q�� ��4�_��`��5��^�D������7Z�K&�;��O��7 ��˴�:;��<ӵ�^#E~�E�a� ���I��/�0Rx��=F�[���F�O�L�5R\�X?~��Fky�Hq��	����\�3Rr�����҄�νAn�L6S$P�čv���µ���2Ԅy����Zj�����D�۔$�r}->�۵�D�����/R����u��b(��b����=��S�̐#Å��ɏ���ms�}���!2:ϒu��b6d���d�g�����&uf��'ߣ�`�[u��M5��''��
���]�L�3�"Y�*Sü��bCGPдɈ,]2�|xΝ ��ޢڢ{n��ui��t���K-t��N��Jl�vPD���ُ�����.j���XH������~j|��Bƨs�b�Fy�Ʌ�$�j��jz����^��'e ��Q�|�%۾yZE��nT�L�A���y���m�ۋ-6a��^������06c��o��x�Aډ���t5#м�=��]}$B�4��G�����=��s����)faǌ"�iza>ش~�{6}.e�s�s|/D>���^2>��Vb;��98g����sL�,�_�Cs�/� ��]+a7���F6��/����LFѽHs����9�d�|q�69G��:Go�?�Fڜ?�;��?��9B&�m�S�!    ���A�Njمn��"Lb�`+�)Êp�B҂g��D��N-�W��\��|l�)X�An/����;�����/����{p���R&��@�
N��\F�����\����j�C�k�"n���)H燠jԊdJ��+���̀�.�j��4y�i�H*��N�ٲB��A%#��P&R)�gT,@��"3q��1R���p�S�� �cč���x��*��yM�D��,���!�JE�/�Z�`���^U�ke����D�$��z�[����'CF-�"W͂Ȟ��K����<չ-�d��3�1�L���3&�(�ݟ�G�֍�l/�E�iEk�%1ң��|H�b}�:-TIV���VWVdqae���Ϊn��52��v&Z%r��<&���}ט"Zi�覼4�~'t���5�l�o.��肺$o�/�[�6��nx�F��H"�H�����T<��W&��Jj/��jfa�xTnzUp6�go �h�J֍-B�i؋�h��-8�Rh/���4�:��3ʼ�+?m߼+KQ7���Gk����/g�^�
f�ڋ*���T��2/��DD�7�Q\�:Qţi|"ě_�?�f 6@F�An�� �6q#����5&�}?�]��{U:����-/q���.Y+l������]�x��e��5@���������2���MD��AvWNT� ��l��IQS� �d��odEeL�ϼ��Ee��L~ò�3��R&�{PX�kw�P�|����.|8+i���-R/�*<��o�*���X*�k��ƙ�k�z�}�>3���r0�n#�|:����������õ�^3e��"�S��6��@��oX*j=����*_B�e_�U�=�'l������Ykh6��̕�Iֿ�W���F!U\6^Bi{e|-��^�M�n�>���p����M�� ju�o��"t���ǰk��s���L4@�J�K�j��:p�^&t��dU��U�D�Ex]��>5^Ed$���D�ާJ9|rA�E��#:���"�iB�����s��:d�\�䐪�T�'RbB�C+��(d�A�e|"��yU#X^�+�.BV��H�)u��X߃A�����T#�;Qɧ��&��PT�e�"\�ɀ�t�	���#�9�A}x�7H�>�^˾@}�?k>��R�o[�����%��A��o���ߴ�糾�e������G��^���_�8;�K��v�b�6,�Y~��"�o�~�z��l�����[�(�Ypl3��lw�_q[1.2o�Q�z㹘6b��5	�a���ÄQn�*���ḾÀQw�(��~q�b~�|��m���֋{��oj���l�A�q�.�t2���]pM��^�E����i|�|A'�Cpۯ�.�EB���}��Zbq��[A,T5����:�'1�ga��(���D��,�.M\��F�omf����ĝ��WܫwWŅ����UI\��pVf��$��Iд
I ��oس��b�@`�T۪#�r�B�������"S�y�Kwd�CwL��8��$�@wJ&��ĥ��7����Bҍ'�UhQhs̏�7���I��� kA�ؐ�/��K�n������C��iK����ܕ�с��\d/���$0"o�xG���_�$b�sA7�]�7��LѨ�G�(���R(��D�ޟ]�$iˠ-^o�B!�O�H=��exX�4�)���T��H���x�MW��l&�c2N�����>)���A�"�w���{X�*"��ʑS��1�)�0-�g��f:�0\��F&�t���>���M��-����k��PBW�o�V{���W$�~���d-���Ar1��X��+gE�t|���\_Km�K����N�Z�g�V�N*�O]߿q^q� �>�8��j)U��++�Ù�}3��s�o-����zk�P堞�k7���LЍ$G�2��[ո�; 9�4�؋#�V�9��xE.�>uD�L�����A�H���c�1�������c�61��,0	���c�7�CBf.�,�y��-�C8؉`D<t-g0"&�������;7�������KԀx�Bb/"D"�H��$�&������B�mS>�^�r���`k��H�E�f�[d^�3�߼i��Eݨ�d�%~)��Pw�i,E�"�FI���f�xM�4�KA�u��c��}���7�ج����=0�=��'в�ntm���J[(�L��]�)��n�M:E[�ފ�;������4�ɿT5u��	���9����3Ọ��ڊ���������Q6O&ͩ���_AΨ����&��s�*��K�إ�,9�T���,L�.�ل�\�m�me�?J�`/TS�_`Ax��tIsE��9���N#L&:�f(�:��v�Oj��>r<dsu�x��;�<������C*�qm�dT}o�-e"�%�ݟ�<��k�.V��J�v��/�����hu]9t^��J�¡�V����wt���eQW�Al��Rm��m1�����@���`�Al�1ś�9�\�MQ�i��D$e�eeE�Vs�t�k0�Z�.EC��m>�ԍ��m�"�R{�(�	;|�(�	��^D��7��82Ⱥ��1и�7ZE����83�E898� '�C�����P'�Y܍v�A']�������@��Jf/�i`u���f��M#�o��qϹ�O_�Z�X��m4�h!�K&�+A�YbxG朦Ou�s�s*��0�f�mŽ(���1sӹ�Nc�Q;��w0���2f�i��49n:�t��׹
H4ŧ��DC
b�ۋ��[����Z�^��]_���{����ފ��rH1��x�b\�ª�e���B��3�Bw.lw��e�Ή������h��E|�O����n�I���B`�p��zcْ���FTh���EϷo��TI�� �r|$8��H̹��"W���&[���6�:�ޕ�L�{-�+O Ȫ.ܪ�Rc����3���� �F{��.x������$[���.4���t�m�� ������k}��|i�8�$J� ���l>(�}�a��w�
;�y�	�8(��2i�g��w�2ɠ�qKM�)珟�;_m�Fʆ��U�*�� �U{�O������������KPϼB]ׯi�6�|�R),P�'h򁰢�b�wn�kgw�V쭽ɕH�|�fj�w�.Y�EML�����K�Ֆ4F:�\�w��k�ng�7:�����%�����/9��@����I���/�7z�f�<n4��W�I�f4�����^�<�m1˹�����:��4M�Nfb�;�"���;
%	]��4������YT/���Bǣ����A��Y��5���5k��D�,��D#�-���*����(j�]�3/�s�`��{��o�^z����7*x�p?/"Ygn���a�w�k��#m��ܣ�T�2��C��9��9�ß�����?��dIHy _�@/y�~�L_P��wi�W��k��q�	����gC�t���8�my�,~t��W!���^7>mGѠ������_E]]:��唢j�J�$�E�Z�uR��0�^Bv�� 6�7�/��ѯ��z���AX�M	�s>�B� O��"9K���I	�i��uTב��[#<�;m�ϳ��b�]Dt*�d��B�v:�H�l3��#��
�w��-���1�D����I�[� �t6��K���UU���J�w�0�:EӴjZ6t"��d臉���N�Q�^�<�s���D�������1/G�!Mx��w��_9r�d����P|�g��J�c��N�*���;�0|C� 7�kU�v�@���9����b�Å"����!����8Nő�Qu�u��8�� {�������6���%p����?���j:x��c�)�h~%��������1��n�q/}�8yQ���!�6�߮��xkO�u��`K�9��.�O+���0���Z��BՉ��V�&��8e�og���܆#�(��m�>~�����ߥ"�.j�G æ�ቝGt�+�I/[���e��z�.��J6B    Z���!~{homU�&��I'}��������Y#mSՓ��+�#Й�Ό�!�u�W�Ii:Q:"K��Or"���K���m��Nc������Y��E�y���_����*7N�WQ�Qy���F!}��ߞ�����5�[�J�E��Z�>Q�w/L>�y���4��lK�U�>���Vc��Q"#��}��u�k�P�2-~ʨv2��G�?��K��c��T�g���\�M����Q+���׼��G~=�2��*(�9YPbFQ��֖��Y����P�wן��o�ձ��T�n��k�N����'�v�=6Ľ
�벭+:��&��f����Ɖ͆(��+}�c�t�&o(�@>�{W��Һ���UJG^����nR������tA+�����_Io2�#nxSOu��C���d/�צ
���{�NX�0G�p�������
�yQNtr][���*��d�E���	;Ci&<]F��TH�������su���*��ɏ�z}�#]�)��g��1��sj�:e�R���e{�	Rk��Aޙ�0��6�)p�x�	&�0LbG���7�kE-�H��}��K�H�Q�#�x4L:<L:e���!|'����)�����J4\W�Q'nj��!Zi�f?0b�q0������}���D�經���|_r�+:U��z���W}���<\�D�y�+��Λ���0CR����ӯ����A��Ҡ���fm�aSU��j:��{�>��j	ٽ�قD��	XSh:jĦ/��5�e>�Y�F�T����!�	|��R�揋��"�Yl�+���^�jb@4�n��{F/9L����>��z��'�/�!ea��o�*V�?%M�M�qW3&d3th�k���L���ٻ>��9ޒ��u�Ն�J9}�OE�IY�J�=KC��N%9(������ps��n�i�T(ʵt��d�xS��ap�.�~GQ�`��-���,{�R���.�<�`��<������w�w\*��
����]���T�<��BK����@����l]h���Q̳���
E�o
�����񞠠��Ŷ�nԆ��K�!I���$�����Q�jp��S �d<�V���pP�4B5 ���1)��{��`Q%2��HۥE��G\�8X�$
W�č�0B:�<��hg�T" .�f{G=D����<U��G��B�ౝ#h1:�j�\�	D��h�s�P������V��a�P�����'���Ӵǔ�б���ϖO�Q�b0Ԉ�&��%�A�/B�jl�4���T#��|�u*��o���~���\f���4>�=X!����y]�,��'g�<��X ����_�k�EB43պҹ.QW!���=�2��{�-����8�J����zl�a�M��:L�G���>�EZ���T|��o��B���Л��Cb5����"ULA���0V��V��UM���2�h*���7)�g�ґ�M���Oun)(auBr�6����&zR�$�(ݺ��H�u����Wv��)ÄR4��^�P��S�z�}w��G[�}>���R���y}�"ȩ/D�9���?�7B<�?����ޡ�u����]�nCU4HWEu���J�O~��处�s��V,,6��~���$���|�!�[�k���b����̇w�TB�0����7!/�^_����2����	��!�i��b������^/�盒<��Y'竻1�V5:dx�J�WK�T+�<�4���Mv<4�0i������J���P�M����(ً�|�~gУ}�ZъIu3�ʣM�")�J���J?L�/9g��*�^�N�d&E��xd��6W
%�X_8^q�KU������o<�����ɾ�z�
ѵg�o_W
EOp�A#�x���$����v�;a��qm�ޒ�+�M~)W������||�ȷ�2Dǈ�ٔ	��b&*����:�
r�ʲ�ʯ�����s(�V�^t��C]B�Ԟ��^��ӨK�#^k���}��r� :�[��*��� 0���Z%h�!���~)]��\'L��Jr�R/)��ԵE&�Lԭ�����H|y��g�=ݒN�W�:�������?H8ɜ�&Ȇ�z��VY.��O����3��M����ڣ*�+
:�\�_��}	ur@���}"!�PpȪ$�Y�8�\i��V�_��*�p���ҏ�w�EOn<�Eh�g�?bH��^i�����{\L�On.�X�T�A�kA�ĩ���*CXSةl�7l��@��v+�\�	7ڈ:ŗ������E�B��V&�����+&�͘6dpQrE����˺�M���[~��^n�zm�%��A�tY|��h\]����8���:+��6핐M�{"�2�`"��I�pa��R,8$�+�k=��gak��Zcӎ�Y�0�
�V�p)���4��f��Ѻ�Ҳ�_�����507��'o7�l��Q�M�LyU�^N4�2:�����._�^�=p1M����MfV[�Vխ�=~�et`��;�%. � �I��&�ŤC�S���Z�88�.����4yu�����U���p2\:��ᵥl�2y�[oI��f����ƺ�K^N�2�)���lo��N����NJ�W��_T9�:0�+K���{v�G����^a�T(�óƉF��� я�x��'�7�ă�&���zC�^�E�s��b�3�ؒ9��	-�Ŧ;�q� G��պ���!|g:n���|�7Ɍ��B#��?�/�i�����M��Q��)�ֈ�SQ�W�4Vgv�i�g�DM�A�n~j}�^]�e������j}V�����s��f�BġO#�(݅�/	:�v,�̨�crR����R�l���~��(-n:F��<A�Ur��%�V?��"I1�]������E�d�5�vg6��k����}����|p��$Ze�,�m_�g�,Ϣ�5���+FY�u�`/����>)i�O���Iʖؼ�ن�ju�R4�u���|���L=�W�|j�3�̯ׅV�i�ٖD�K�l���^�[,U�����n5�z����?��=�VTE#Y"iICX!�}(�U�R)zـ���R�h
��V
t���Q*�:�D��{kF\1pr]XqU��a�G޵0Ju�ыG�/k�E�E6 �_:�Ce�^1��i�?�&�2��tnk�z��9�����eׄ.��Kb~���"b�Tr��6{'6;x�|��ӻ���h�����H�ؤR6��^�u <N��ѝ��������9��L��땝UEh���y��C8��ZE�����Q_�ťs��B��o��9�7�[Z�����^��o3��Q��}8�T¥�1��|�@3^Q���NL,:J{Y4]<����H9��n�A�hX)�v�ԒѰ���^3�1��e��[���~��4�Jr���6%��׋Imܹ8۝~�c^����V�,�z��<
R�ŝcE%�
ћ/���Z�Lu�U4�T7�g�E�ty����3tW�wy�oI�*��(�D�N,b��R��C]VJ������s>���������w/y
�,g�,Պ���j�k�RWG�q|>�/��_p�j����-�*������sq˥�&kUL��oMIat�+����±�#თ~��6��*,��5s{�Z��_V����q��ɪS�jҫn�k~�k��u���fP��J��KT[A�A��P���^ LV�x0��WH��e�z�EM��'^lR,e������1#=Ċk%H꽴H��X;�!mtf�k�f^48��0��If�\en�[�;���Q��͈X�O��մ��p\ݐ�\L���Q���B�X����w��i1����zӃA�hFW ����`.�6����DS�t�?MB2mD�@�T���V~x��S���Y�U��x�?l�VƔh��*ox�.Y���\-a��qx��j��\6*��Bq��I��1W�G^��s׵�m����*ځQ�H�v�-�mU�MhC�=��\���.�Q(�y�=T���~���Ӊ�/����~����b9�]e�gP-Z�cFD�`�jm�ȴ�T�"��%��\��t��W�7}�����ע�)�8NǍ�Bh��'��    Q ��g��:�-%���Α�'L���K~��a�	H?5?�_�*D�کE�;��E���k�~��}Էz��Q;�5�d,�xGI�k����v|p8�m�7YUm��п�Q��{κՠ�{Y!�[�K�;MPr��Nh]��"�^�x����K�<hLV6o*�;y��~��EU���2�}���i��^V�e-��j�Z��h��-�/U��)F�K�p����	�w��P{b���%�ͪf,�zC��CO>�p>�/�&n���Zb�<����i*)��$k�6JW��\a��8���'%�4�㷓�!��XG����� b�੤�_�y^�Jr�"?�B������^����^+���[!
~�?�}�f������R�d�.��q� W(Eի}�R6%��9� A7�(5��+���;��UE�2$��Zi֍T&G��q:S��M��6k{֪-<�x�Y�83%�ģ�E�B�~��m��?$�ѱ�}�x������L{e�$�m������Z�V�:C5ӊU����Z< ���a�+�~�Z�h}�c۩1�^A<1w�����K��x�o%(��ɠ�R�_*��7}=��^}�^bP+��wz�Ըq�G��A3�8�Zң}���H<��f㴪�p�x�l��8Dk�����%�}��4�u�<�͕7HY��7ӻ��=�����x�m��7�ZV�������
 ��������\8�������?��������B�4����������ߜ��߶q[���7��>r��$�QU�䥎�Y擜����7�Ƞg�P"-iL����r`c���s1��;1S�_�c��'���3,���?Mz&�t{���u��2��@���Ŗ���ׄ6J�z3M�#&M��١Aח�O5�wB]���:���'��P�bF�u*�975��2\7�9�;�aü�]����]tB�T������C���z���>�O[�c�������Q�8K�1c��u����U%��.����=����
�R@��W�1�hl�E6���hk�k������qvx���Ѻ�2#QG�F;Vɀ�$6���~U���5{��(J®2��9���RMq�Y�yY>?���t�"÷�)VB+��2���\%�ɟLu��_������!c�v�Ö���WV)�j��{QK/q�I��S�C���(n~q럢��ycA���]D#j�L����~�����v�2]�}�V��Ũ�����
TR�լ:�Ʋ"(�f�-0��������G�B"8vg�X���BS���+C-�Y���!��R�i��boԘ�y�u/[�mH��2�Oh*�$Ķ.�"_�^Y��H���ӎȸ6�.3�麒��$*��=>2��x�[u�|��GN����;6�O��������*Ut��=���+�f�?]?=�?}~�ێov��m���l/��8�{�Qos��Ws���'���Y�w[�w�`I��H,yk��>����>�/�񙫏~����a`�O��o.��O�f�h$cc�mQ𞐙EC��*�en���K~�����z�Ӄ������[���[�}�U����GLc���������q�L��mZ����W�P��{� ��Z��3z{�7�JoX&�]՟m8�=�\���<r�Q'8�d1��A|�y����w�_�]C���j���F!�-�nK뿛p��ws��TԼ�6�!x�_GbB�	���p� ��d��6g�9��7;�_����Z.0�	 uެ�5�ޥ�<ղ�pJG#a��������_L�8�t�O�v�ׇ����L�@ĉf5G8�bR$T۶m���5�l�T�8��-m9Lrt��(�M��1*~1{����?IkB�i����dNNDĔ����%KE���/Z�cjN��hr{K���&�h�����cN�'jgOk�~s�R}��W���'Φ��y��-�@�r@Q���Sĵ!�m)�{�;���x>t�\T7�C��,�����W����V��8�S��3�����#��@;�"�V{��}muB��nd�֔vW�C��=H���x����(���$�!2��Ek�¥ �8t�Ϲ)rL^O<mS|mx���[n�R|xG}!�����#q���~��/'��뿇��4�e�����s�&�[�}��h���[�
	�4���t������Y��g�'������ф~F���N��2Ad����G:��,�^6%y8�Q��$�  ���ɔ�,*G�jI�*�u� (�������p��K����cb8�������%���B���(M�y��񿠲��̼�S��
߇�>�:��K��� X����|d|ď����A���}����Ǜx�G��+��ʬ�P�]��#i�űŕ�=��j��	�2ӓ+���m�
P(�,8 ?<���;\{Y(6�l?U�z}W%���}�]��IZr�0}���c{���3R�&ɵZa`�E�Mf�A�jP�J!�tp�� �88k�Q�W�g��TZ���i-\���ЫBӕJ*�����/�Y��RD��FД������>���8H�q-�m����'��N��N�Q��":O�o�5���FrAQVQm�nr������[�A����p&[�avM���U|�f��������K���)�H��>��2dHaopgԴ������"�8?y��%�	��_|�H��T�Bw�w\?>����8"�(Uz$�K<L.���]�a��[��B���ؽ��#�~���S�yoHn���^����6�{���[�g��dN����yp��-9+(�����>�(���'ZC2\j�n.�s���7_���b��}���N�Y
�G!��Q�)-�h��?C'_�Y��V�;^��F�˼��w��1э0�h����W_�=nHOz����i���T���#���=&�2�!+ڊ������a�?�Q�Eå�ǳ*�f��{z��11_7L�v3�I[6��6s:����V���
ij~�>Nf��ҹlLn.x_��Uoq<���cw��3o���=:D�F�Z�t[��i2
~���-NF��AʮˊXl�(��#�jPJ����4[�Q��|勶[����04�A񹸯�7k8������<1��ؒ\���u��J�M#��N-��2��#�xib��ٽ��;[F��[?H������,��iLzL�B��Dg���t \Dq��,��S@���F��t����7���>��d��hw��
W�VN�u����!�.���/D�,���; �ܩ$+:�����B�6�����!�EVJEk�;~<h�yM�� �ʇ55.
�J������t��t��G����z�������*[7��ֶ>s�;�Uph���挢�LY���O���5��eg���r�KDo���I��ɝ��Sw`�_`�)a�u#��.Y-��� L����*Vw�5��C���D�#���T�*J�h���#�f�U���d����)���}t&sRGY'@�p��]�����"��*�8E�1�S���nݦ'/�zS�!�Kc�a���t����+�v�e�`Yv�J6z!wI�&j��"OE�b\��w5��D����D�nZ��檊�O�V��4�����tio��!uӠNpw��O��N=n�Bn���U�XnS���$1���ʢ����l��"���BҘ�4J' �CMx=ʘ�Y�~o�'�9>��|�8��=e%GyT�j�ꌗ���Yzm�������Z�k��c�dB�SzT�|�Q���,4}�$�x	�"[�[�D����FIv~��j����	*���I�ƐЕ-�O��0N� �>W����|@��r������JޏQ�h_$�L�x��7	Y��i��):7s�ع��������[0���y��
hU�U[ɢ}z���aR���$Q0uE��ӧ�h�n����%�����ΒP����Ed�U}�������Yo�YHL7:�j�r�xa/�]���=���2�쌂^魽	�Lck�N�k6u0�}�,�LW96/G�d��[�8yd�Oȹ��HN����U�.
EQ}    �ыn�K\�洽�y�i�����@
*ʈ����t�&aoA��7�3[$�,���f�E�`q�5{�m'�̒U�os�E�4l��>�B�%�sa�v]k��o�i��z̛{#g��Z���e�	���1����5�?D!U���(���Ѻ�d�F�:q�0���ݘ�e7fIs}�ڃB$Q/1�^g�b�=]�%7o�,
��h3��9���ݒ�g�(e�-�cC�`�Cg��֋�+����b^�<�|�sq�tR��k��K��C��i�p<s���v�߽�#�<�95#�a��l+��ENI>�E���:�R�"'����ܷ��_u���ބ���\���a��D�;�����w�����1�˥|�����DgٯP�RpVq��0�6$�n�2�o�f-���N�f�(��(�L+F���E?�{������zh�1��'Xs�x,��q����/P���}uC�����7W
J�M�ʥ�*���}�a�RsP��g�&i�:��M���bM�b�d���1��Ѕv�
�0��}i��"�h��:�Z�����qA9�v�����7�����b������ N$��(nu�j�#����JƩ)Q#�+?��̭P*����NH�t�+.��m}�N��b����?<�5������ǆ��㨚_U���H��ע���&�p]�_��Ru�<��r]�/1>t��K��M$X-JR-��v���}�������5R]��U	�W��&��!ZN��Uh'oX2Z[����Ը���~>��]v��,:�ou�G��'3����q�i�#&0<l�����}�U���ᅫ�֓zLf��W���A�[�&cEeڀ�t�@rV�3�k�i���=A�U�mɂK��4{�F��y�hͯ�M^��Rɲt^�D��I`~��}ᗦ�9 J�x�Ţ�J2�{�L�s�aA;���"1�]�ӊ�~}rn����H72���zeW$R��F��o/�85D^E��r�G�c��3vt����w�:��n~�9c��5��Hy4���u���/F�:K�ѷm�����w��,��E3��;��
�� F^��[�oƷ�4�ۂ!�G��B��&������D�:�^�͟���9=@շ�F�A��]·Q����Yڏ?�p��A���`Q�ǤXr3D��ꄢsB�,8���x�}3�����CF���RQ7��k�֌ܚ�aR����p����צ�1���ްV&�T�<~BJ��f<h��E�B~?�)Z�n]�D��B�AͤQ��ޟO��X�)�P6+6�s�Qҫ�9�{C���@L	x���B�h�S��m�Jy���*l=�uS_���ű�b�ڐбʡRf��~=���)�xc,�lPޞ��!{�I\����ꙍ#�N�d=-̣��`��\���?�Y�h~%�;�p�Pq�{�P���оۿP/��֋믕>�]�@5��֛��A
nJ�zGB�X�Ni�ޕhɕ�R���g�����/�Z%Gz.��[~<��eZ���/un����#rS��9������7/�R�5P������"��R(Ź�ڹRXp�[U�0�����@�xRC�����tҋV�׭��E�q��A��{��tb�H��r)t�0ٛ�2�-{��8Y�D�̪E1����� ⁼\겭eשׂ%M�xq3��*z��zt����x<�R$���p]�q�b��oV,�$
�����!�7P���{���w~n6g�a�4
>����v�K���XJ�1h�Yn���\i��^@��K��.�e���%����aW��^?8ӑ>��m�Nu�Q��q=�^�s�"��ǰdP\Ѵ۝S���z9LZt��+?���Q�KC�P�V�UٞȾ��=��q��M��p��z#Wl���B�s�Ӂ������M��ֆ�Z�]ó���m��.:�F�o�m|­���O�2��hJ��'��U��wg��6j=���=�J�&��ťT���F�wM�y�HK^���0lV.9r�=\�.G�����U0�C��wnZ==��Vݒ#����J?r��J������$����#d���[�b�ʭ�:�?�s��4���t�6vB���q��͛�����r���uሩr�l�ZdW��fUL4li�˦[��o�گ
�N8�#7��>'����[�9��F��cIR��T����o�����U�K9��z�¦�얱 �wl��>�KS�܈�::uʙt4�����U#L�V~�sޥ��_��󃫅�:�R+F�0c��R�l�?p����j�Z�0S�0E��E��Oc�"�N�n� ��`�&�e��Y̆�yk��c��7H���;5 ����]��8������6c������·�[���v�~WL�x�Vr[|��S��s1�(�{���?�s{�	�~��xɼ\?5��,��_���)�m�2�&D뛚y�ڊ�[�G�4 ?��>�28�4K�[9��#�c1�-�~ћ�`��g �L�v��<8h�D<~�9�wF��y}����?��Cc��J�T?���w �h�s��V���ll3���]A��B�Ԫ[434'H����D�קfn�H����ח��v-�e#Ε�nK��q�f�L;Ģ���T'um��6㾓m��p84KV�e21���12,�.�QZ�["V��$&ݡ����)/�mz"p�ҞZ���=�ig�w�=x43���գݍ���jZm��i]l9�:��	*�,�+[2�iC02ɋlyښ��r�xcT,[���I���6hv�j�&���j���GFq�L�\���)h�1Z8B�+��K�v��r]!:���Ŭ�T�%g3��$��11m�a�bQrGn>j�,{�a�xG�e�(��l�m���-&��zm!4lR,J��+����nĴ��E�l�����,M%_�TE5?2h�Mp�H�t%���c>K��!g�g�,z` v7�-���DuMk@*mo���ؐ�ХP�w:[o�Y �SRB��h��{nˍb�N��(v�q��7u�U�=���i{�%�qm���)���:.�N�J��TFf�95w�Ya�v��������H2%�b˜	��Ƌ �O 	� ��:�"M{Ga1�ȴ�:�Ԉ�ˇ�~r�ezʱ�0u�m~�/���l0�'����t�qTJ�N�Lm(M���r����^#Ӌ(�"J&�-m(x!��`8>Tz�i��ukÞ>�g�o��%��ѫ����B_������~���߾���ϯ���;�;D�!j������۷�;�nȶ���|B	��C��U����X�*�AS^���
�H�˦7p���-�sj�Ƃ*�x`�n����㸐Yy�� ������]���u�#/����!)I�W#n�l����|�H\QQ\y�?~��늋ns��.�ӎ���&�n���u^�4xɺ�%3^�	��.�ӚCy�?ݩ�5^���]]ιZ�r��������^P/���N�8�u������BT��p˱��`��M�Ǥ�F�I9���T���|x�̱�s2���޺�nVfr��&�w�A?�:��i^�������2i�L]��PKvv
+�X����*m]��<$S���dR��9��[�._����4��O8�N�{r>���	S<ay��>PL�g�9Xgk�1����2m�x���|�i�h�>��E�6}jL9�Y�n�(:�m|n6L8��z|�1\{:�d�ڐ�F2M9�X�n�ά�K�9��u�ĺ������ӣ=�$e�:=��u.���K�y>���e�K�{����#RszǢ��4�8��l�ՔZb�-3�0KG>嫚/��2����{k�.�2�Bq��SR�\�7�X�nOC?�x�L�E�G2�'����v����5Ӕa�r�Muy�3L��t�JL�8�����9���>q��/�2a�^̳����G�?��;�����GwEwޕ��3��7��>���Q�[Z3��JC�+�r�i1^���ͩ.�i���o�z(\MS}N��<��i�q���2�,�۫�i��4�l�z?�E3o	X��L��SSs�ߍ9	��!��N�7!�$�'��!�G�L�k�8c�    ��}���4�,غuF��#)��Q%��9����8z�[���y�����5�ҥ6і��6��a�]T^�b���
�ǎ4U2g1I$��94�����H�
�j������*�X���"�e5R�<���t:ocr����X�wqU����m�TqSMMF�:%�f�:����ޖu����tqY�d�r��5CR|.?�d)T�#dmF~��iss{o� �i�TqC����2�^��X/��b��:c��1��LWi�Ha�� q8l���:�'�ߍ��-��^���'�_h�t5M��i�i���^�sf�_�t���I&�$���I�j7̈1JA��~�6N������Ay�O�1�]��{�V@����zT�������jI�2��".k���D� �k�z�*��!�t%�����.SXݺ�8�WT���W�x�[A���34�0'�J��&\�w:���o~���x����㶘lf���-C=HV�+m!��H�������P��ԡo�X>��pujgwZ����V�����,����l�6~�f��d�a�r�:f]��"�Զ�)�ؾ��|��A���5ת:	�����e�;,��UϿ8OG�Js�(u�q�X8����r�-�ݥ�3����<���	�z�t<�������7���^�^�������7V��k�n�5�R��}�EJ��w�a>��]�p箋�~h�\M	9���
}Yy,h�r1L�'���;W�޸ס-�\<�-R�G����ɡ:^���m��_;,�G��(~Es��ț#�ko�49�Ֆ��8*�r2χ������Q��G8��M3��>CU4ߏ�6�~�08��@�����f��^����B͕V������u�x�,��_�s���5Y���{�d���}�4M-��\\�l��(~;VW�\���+C���բ�B3GG�U5:"u�(�d�W���yޫ{����n����G��w�x��q�a���<5��Ub�?�/[ry�Q��u�Ʊ�yLe�(�9ӊ�}���t�C��լ��f�H��-�S��z�1�4�"_��}�P��yӒ�C,��������Fᢞ>���Ø��r�ı_O�VU{o'�cY�z���7$( }Sg���p�5W������9�6��|cp�G�ǱƔ#�xH3"�_�ĝ?��f���3S��ىrW2�'�F]���p����>e�,'KtS��RW�e3�Į��g8�u0A֦=�p�z~�0,x�\��p����\�d��:�&����]{��xޒ����֦��f���%%�y��eS:����O"��_��s�poa9=�t���q2����vjS�U�,F�p�<]Ƭ���bwH��8U7�2�!?�E��!qM�w���\>q�˃.�S�8��L�N_pt�!�ܓ5�9U�)���v:G5S��m��S��0�K5�|��Dn��'i?Ł6n�7�V�\��yy�6~K�i�.h��`uVۮl������p#ү����b.O�Uݖ�~�
y�S��Iy���X+���M_�U|q�����[3�Z=��L�2���yu�9Ǒ��c��W���=0���l���h��}iKq���UD	���יf`���,L֝W;C�g�$]9?n�>�WM��u��OJڌS�OL��C�k�n���.�WbI�4՝.�Q�.����Z3Q�2v�h�W�j����
�#����.�.-$���D�K'����rW�z[Ky?k��Z�pn���gy���<S��M3u"��RW�w�DK4�l��'�<�uz�F�N�1O~�5S��VH1�5�l�O�'��o��#��OiT����r��S#c�sF*ϐdlb�G*N�,��G�Vi����o��u���*�冕��V�?!]4�Η����ic��._suk���Ռ�bK�q��̛T���6y�j:�Q�j�$3��K��9_&Eu� nt�����8˯ W�l��R5;8�_9����GqRu�f��e�Ĝ>�u��� ��i����G���pA١)i�"��_:5�HUz��
V3O��d k���ع���UQ���D@�`�r���}v�J�>OO��D�<u���ʃRE�h�T�t�_�tz��O���ᙦ�E]^;g����/ڊ����`�N��زxa	�����'�.��;F�$>*uh�i�d0ߜ֞,]����	"B|�����'�M��<rx��|Z�.K�7��y'��5^su����[���JK$ǝp�i�����������2p�xNn�ūb�O;+�S�����5�л�t�����co�������j�X�����X��A��祁��,v��k���R��Q��9"�{����v(�*�	7y6(X�S�4�jq�'�^��~��Tթ�(RvΟʄ&�[�lP�nxj�4����L���E��+W�f ��ם �pFf��U��ޑ)0\@��1��+fN�(�+��%-�It�M�74��s2�ҝ*ݡ������:O�v�pt�뺕-'��;����,5�Fϻ��P���iK �9���By��A��r��7����9~�.^�P��.�Y�b�f�������Q��oG��h��x�en�b�3=5��sobr,>����"��|tH�f�����d�rr�#�����
&�33�'��v� E)����b���yʈfz�C�iv�r��X2���ɝ�F�ː�r�A>��D�����M�k��i1Yo�����Ӱ��r.U��'���5]qu��Wu1�&�B	|�Y�{���e��;Vgq�>TNb0L��o����-}�  �`�G{����_u�B�k��O�e6f/���Gq�����1ވ�S�O�߇T �2���p�6�O���jֿs5M:wW��2���g�j��F#�Gq���	_�Qk��ۦW,3p�y�#����q�����-��suc)�5�Μ��,�xZ���\��p�O2��&��itvwQ�@ե.�٦��L|�˟�nW���O^��҄��tU��v�'��ʰ�ґ/����7ʟE��bb��f���˹�!��i����@�w�8
�<�׶�ՓG��[ ���e�����e�@q&y���jҝ/���D^>� ��vҡ�Eս:<�Ϯ8;��_�U3�ߑ�H�m����p��TU�{�\�<5��縺\;_�ؚ>�c���QDZ�����Ng�h��D0������b	�!���R���yz��!iX�mq(%�>_�P��������$*���_�&��2w7ƶ.u>��qS��&�{T�q5���V|!� �>��~���4�4g;�I��2�m揜�������P9����<Hv���(c`|A*Q���a��Z*�I�����RW2�V�zkD�4��*p7��4�J鮃6)�*�ة�|�_g�j�>x�3�{A��r�k=���K^UN�q�~�sv���_l"�\�����7����*����x�r�����@�k5������TG��:��莈�%+F���8�[���ҘD�7s��/Ύ�rҜ�)�隙�WE2�h���z�_.��MQ�s?ڸ�����$�(���:�S�%[���繺\,t��������37f�-�u�ȩ�4kO{0�(���Dߪ�nO�?���6�0&4gS=��X1y"ptin�Os����&Q���ky���G�*�;��{许�'>�k��G�{`_�#/�]�U3�}�LݥS���v͛�
$5��x��v���KC��+<l/��^�u�j������i23��K��+����Wg���7۔c�T;.Tb�C-�x�1N�� �9������#.���f���
��m�9�1��d�tz3׎�#��E�鼽LsMB���o[w(K�.=���/wr�ό�����6R�&�>�
B���e^��)̝@���x�wF�Cof�I�c7z�*�o���@��@5Ց��(�ʡ��A�6�wi���YCV�M�[f��\���˽_�`���5��(���t�*O�l�j���\�ޥ^+���CAM'`H�U��MTݞ�6��9l:n�[r��H=bd�h�J�_BF��0�A:�g�W5��    ���'�i��/����t*�u��icL�D��(��AT�li�H�A�������P�ŹWL�n���r��*$�"G�mӟ��q�q���� y]=��γv�ҿ�|�f�g:1?��ANM���d�3���86��-���m6����up���B5<D:�n�S
�a�)�U�|^?=� ��-2L����8�`�:l������a�<%Q���G���/&%�@vؗE�Y���i��Ԟ����ܟ�_z��]�vt>�t�p�3]&���[����sy��nЫ�6��%p���r7�epy�|��"�bw���g��7�:%�f���7��&�����[cI���d�4�H�{�0,&���Y�: #ϡrz�y���'!�g���c��awU�X�*P�am��ש?8���w�����[Oǥ+_�K�N�(y�I�.Fu��o�'e��2�E�o����^�睺SY@�n�(}���v�MAMq�y�`����>���jy#��8k�5Gْ���d�+_Y���IG�?�bW��J\}������/¯c���9zv]/VȀ�:Y����*U4q��W�G��̝���"��'ñ{�x����QZ�O��cH�3L�s��C3�lԣ�׹�su�G��Y�N�\-Q�~`�mn#j:\CMI���&�%�n_�������
�y��F1/G�ͱ8���<h�|����e"���lI��A	*����mS}���U���u|9!g���c�D�ud��S�J�縲H�'�ݡ�9$.����a��׬Y:�;���>'>'�����~�c�ι\"��Xc/B�Q���L�,��J"����2�cK{(��Q|��5�`.';�<�Z=%��T��ă(	�WE��.�~���b���M�c����v�nk^����!q�O��V����O����#d�#����Qd��DO~�$��v��_�n���.�'�IO�����?��όiJ<\�І?p��s]���/�CY?�����f�6) )-�n�a¸I��7mn��D��R��d��mp��-�Z��l0NG�0�˦����=����v3���.�}�Tt^��&S�<U��\_�6�6Ҽ�]Q���׃`���t�|�^f�&�*=b-��[����־�~;M�ߨ-Z�!�1�X�N�
�⣑�����J��o�V����N�+�)�i���$>�-���
*�OYz�<�ԭ4�!��>��쐠�:�l,����N��8KxK��y~W��n���CG/� #@���8�J*��g�'���L'�?@s�X����GO�R+�N�KU�j
�Г�%o�{�p����+��)�=���Cd�沖�u�4|)5Ux;�|�*�l����yg�l��t�~��ũD��a��dc���7/�]�i�t��v�a;��{���CQ�(��E�Ԃ��;��.��j��%H��̔�V���7Bx.�eZ��o"��۲��1;����=�J5�����e�_��O�qґ�6|�꣺��T�{Tؓ�ʅ��5
�ڊ�ӂ�O�*i�OM����d}�V8��0X��_8���An*�� 6��f�4��Z��N�T)%�x`JVd�p[>����Mғ�e�f��v�n峷O/�v��ԅ��V����(��>�k��M���{�+^��f�vM��E�΃)��{���.X�<2�/s�E�G��=p$�C���<!�SG�o��Vz�� j̤<��#SF� �W�N'�|?sT3���<�,������⇧o�9�-�`�ԉv�g�øHb�j�Ts%��Zb���0'�]����Cc�'r�
0<Ky��ͩp��h�<����,]�QU2�h���b��'���7�ai�%�	��F�������%��6
RHg��P�jm'�л�,����ވC���QTCw�P��^�ž:H�_Y9M��]�M�,������V�u�a�$-�=��kf�[�+.OJ�0&�������%�p�b ��f��Ӄ����a���	��s�"��B���+Ҋ�C��P��\�oS�S��ײ��t��i0���Q�1�H���:��쭳��_0�|��ݲ8��em�͸.�7?;7J�g�Cf��Z	xƚ'��g�?�7�7D��$!�խ�OUG�I�6VeOG�r(9��v�����I���7�t��?��-���9՜Hus�v�t��Z>��S%�����2UW���K�Itts����}c/�l]wu�!M�x�>0����.QQ�5���|X�յj������y����$8��x��h�$�P}������T�>����4� �X:u��<��L?K��A�/�Q�>-a�q��_������ۯ_���~������o��	�<���?��������J�~�;���8�P6RL�"�7~�t6�sd��#]]5L��!�B���$cH�ɾK	����7U�~!5�`��L�:�p#���˥1Ok���,5�v�LMS�q;}��K3��������E���[	��ۯj�����@��o����o_���r�wY��p�:���6�EZĊ�(��,	l�v���K�ې�|���!�W����+�A�>�-I����<3q�6�<7��I�8z�Ȼ�Gy*���[�B������������������_߼����t�>����{�{�.���������?�S���*Ԡ;mI��[�]�> �yGZ�iM�op�<f��a;������?#,����e��]ȑ|+40:�sb�{	A�K�����d��y������.�"��{�N�~�B���8�ӷ�/��:b��4�?������?�N��"�U�� �Z �B�÷�`uu����N�����?��������Tߓ"�O��������������i��W]d�rC�_ ���4����}D	�!l[֍*q���~���q���U?o����^s�Y	�H~������y7R.��<$�q����+�ɖLVW��v��A,s\���S��2�9���u����|h������{���Н�Zp�!�O}!�A�4��xf����_$Ə���)�_��48�Dvߵ��}/∅��[>i��)φ�'3n�|s��.�X��������������x4�d`�Hl���/������i"rCU��tM*I6;ol�
��5���5��T�H��E���$#�L�e#�Y2�t	Gd%Af�����Y���Kr"����N�I��MrF�����H��I�����D�}�)ʉ���&3ˉ�'�"��Um��{'��Og)%*ǊX���k�6�;�
�|i�-*�iM�JF��m��B]j�c%B�"_I��K+I�W[~�8YJ��t0:�����eә>e�q۟3�E:��&����&�i��M"���y��#���I"࿽�I}���u�$�w�/�'bI�R��K��]�����O��J�%M��R��k���ي�J>�|Eg%d�#ܒ��ߠ�"�V^v�F�{k����_QXa�ܸ�1�Ci�[�k$,1�����r����t��w�j��
�㉸U���6�Y�dA�ÓVa@�����#zIa#A�sPoJP@F�0�	
��ǵ���,�%� A�	�O�G�����59�[bI�e�N��(چd
+���d�(Z� Oh�/�aɬ���į�-i0���@�K����#����:���7�P_��7�+iA�7�2����D��9��1�l���T�>��@���pK���ǵ#8�����M?z�~�|[�`ĳT��ng[[I
�H#�����,G��$V���S�ےO�0�-Q���l)Kp9,I%a�r\�m/�f��D����,dį[����+�o��D��Q�����F1��7J�5��}�N��[��J߈L�Us��η�Z�d��Ԧ�K���,dv�H��"�X��"��An�E�|/K�H�5�$d��t�Y��F�?6m�6w��eʥ����e�-I	 )d�G�IK i��pK�`�hr�Ծ)u��^����m�&��Xɦ6��O�+C&��_�hK蛋��c�x��j�9�lI���wH$��FnB�    #�J��d���$���"-{l[����ӄ�G��ۈ���S���&ȷ�B��~q�"bD�"�#��L��m��O��j��4�u�m�H��X�H��[�FJ�<H6R��Tn�%tc�ˏ?!dȀ����۸��+����r+4�T\y�"n�ֲ��/����`	��#61L������S���_����+A5Y6�ц>¶��9���4u�	K"i�tί����|����f�DƂ��Rs+Բ�0H���p�D�M���O?��o7!�G,�6vy,���L`�" C�D\G'`��Ί^���6 �����E^&Pq�E^.���-�=y�"h�_��$�HOg����߾� C��6^d��D,\������"%W��|4V4�Tz�b��C*��66"�0bi�@� *�ٶ[8��)L��q0������P�D� �M�i���($���	�����HE�@�beBk+��\��WLD�3^'�y%�D�N,��V3K� ũ%C%I)+a:4~���!W��ɧ��j�i��&MeA#M�F65U���ΒUky�Y�7��v��ĦiC�X�Hӆ���{cwz�i�y�j��B�'�B���V�d!O�
ϖ�4VxK֜nz(��T��/o"�l��m�pB�`~l"L�G,H$��o�)�5��D����Q
b��E		1��V(��0?6%R!$Jt>�=�ˏ�h��!�k��ocEۀ@��y.⬑okE%M��׳���LS�=�b{�qh$烴��4#<��P��Bb��=w�N:<"L�5��W��UPzFh�%���z�0 ���l�B�P@��xl=��1@��es���E�T�bqKs��8k$�ZIs���dhT�7i�g����|Um�5;[d�d�ZI�*"ÎM�)ϖ$J	��ψ�;pJ�`�|)���8�/Z۠���4B	����HBg{6�E-`^(G��x-0fhX����ik�
��cq�F�J"���=��EuL��6���Q�3Q��E	�j��V(�T���D����H(�H�5�$������)�|��6VQ ��pE���
�(2R-�*	�8@ږ���7R�[�����'Nj�t2�H��Tb�|M�Q�t9��)�"�9�|q�rDE4d�"e"*"�5)\�J��+��c��8�H�1�eIOr���d��Dn���̲A�ޚ�J8�ˍCp[:Hiǹ%)��,���V���������.Υy+�<�����+WY 5���,4R��,��rE
���J��6�~[�H1ǹ%W��-�t�5�$W��Ǔ}U^���IYi�d�!K�-[y$�҅����|�q���!�/Gg�12F8*+)C�_�3l�YJ��a���z� ��ZJvG-H[J&O�l�v�$-��U�M)K��'�$,�2���t�^.O�����4F�J�d[R�HT���k���	��nI�����R�l�JI���'��\�USJ~;%C~δߖ����X9
|���$!�8HQ����2K�P�h��)�y�%
y5�qn�T�}nK�Br��$Wd:ǳg�7�N�0X6ޖ�0diXq
#���,!�7?6Q��dEb�"d�~%��0c0ޖ8^ɤ��~r�|)&9��e��%,����ik��L��aOΘGYX��+���Yj#�tM$}�@���q��G�;1E��Ekh$-�[M�K9bӼɶZ�H�&)�| ��E��;��,�B�>("�d���` C���� m>����Pu?R�����'i�,���lc�2ҹ��Ixe��.i�2�K�Ʉ{��� ����u�M�2�-2��TJ��f�YʃQ#nM%�FzXB��N��,t�ʵ�$F�b�G��L�0��C�[���t)�h9c�H*K�F2VH�
����~���/��jB���dI�'dJIR%d
+$��<ZI�XB	I��dn���Q�|�|���X�b_hm�\�#Q�5L��Dt���H�ʷ� }��4@�\_I�G6��n�C}R�8_6����n�R��!�9��
1d3�F("�Ek"	1()�ܶ�u��ҽ�P�2�d�,7P��垁�
A�+\ �}���.R]E}h�ϴ߄"����!�B���)�QN�˚J�-'�V�����φL�
:?�I���c%��(�0�S�he3�P4�c��sүX�-�>&�><�k��z�%b�8`�5���� 	�ޡX��'��%��t7�Jg)���?�����
)�R�f[��D��^��H��/�6,J_�K�f�C�m"-��uK�B�Z
��<�$E����p.�?�O^|�O�-�&��D>K��l�-II )Q(���"B��d�R�P"�4��6.���Z<�[u2���6fqd"e�#C�'��D-���@���~i�i����S!V 㧑Iͭ��4y��ɚJ�ZR�u�C׈nCZ�/o#��<W�m��BQ~l"P���J"���5�вl��{�Q�6�F(
�<bZZ�����$l|�j6PT�����Q�*������s'�D � ����"��A�P@�]R	�B��&��R4:4h�)_[ �rM���q��+����d��2����I�ܷ^��2i%�Ja�AW����H�2�z���l�!��RM�P���%Җ�[6�P�� �ح�[HAd� ��7����A��@�ՊJč�%��u9'��G-U�EѲ��Ta�,��T!%l�-�R�W�J���JZ�`-h�X���&�� ���[h�┣��,�VU�l�|����T|��֤�J#��N���D��d���v3K&u�hm�QpD<\Y�6�C+K���`�J���"��7�YY��,��[4�@+Oy*.��Z��B|�E�h���$�H��L+�j�.ԦZ���%�8".T��[���T��d�HZЈX�:��� ���� \�6L�4��ݴIል�ƹ�J#N�w6"T]�C/i�z�a�no!&a"�񈅩���0�)��sI%�#<wu��Fآl���(�x�b�om�
�#�!�ѿ K�72uu�{�v�w�L:JdA�}>�9�폒F��%�y*���	��]-�P����<x�̼��^�&�@�V�@�cA�"��C�A��)k2Q��H�÷�Pu�u�%�T�?�{mk�,S��<�!Y�"^�4�B*S����R�J��0�3��L���d��&|P�>�	�"��A�US;|����O�R���a�Ǻͥ�OSP�,[ �Q�qm�0
ral2H����(�O�R��Y�0mk�Q�1$<D��4�C(
��� ��B!�Cg9Y"��gq�HڨF(��-0�C��G)�%�dI#N7��d��ա�B�lql�4����X�7�,��-0$S���!�<����e�06}�/$"J*�zöֵ��B��-�[`��b,*�I\�Y������&���Ǫ�<Qu�0q$0_�ߞ82��d�đ��on%*!�`I��qC��%���Dv��|uy�FE ��
�P�I ��S5�y;�P�I����s��1T������z1�P$~mn�JqT<t(`ĵ��E��ʠ�^ЂHD�~�h<O>
�<�Fe{�-C���G�z��v��V�0B1�]��Ř̨����CLI�!C�e{��T �AC]��jq.R�g�T"n(=J�������ѱ�K���r(��ss��H�?$�1W`g�De�<W�g��x	|��G�Q����?����}�\}�8��"����eO$��Z\6��<1���@�Hb�O��,�)킡������?�H�/b�e%v�d�m#���ZRd37�Xa��ƘeP������B��A��ݭ�g�)�92$��L��DF^2� Bh���Z�����r4H�����E;�
S�e8�^p�"����Y�F|���DE>���(�)y0�\���2���������HB� {���gP\�Ň\��SH&>G�,��9�����aP)8Dc�[!?���"�K�%:|Z�yg鄉��;�~�R��>+��K�dk�y�B�Y���A5!�rh��҅� ^����`��<� ]���-��_E�8h<*7(,~26>�0��CQ�%�rȪ    ����!�:����i�2x1��%��^�ߤ �2*'��!����v�pdt(��5�D.XOj����"c�ȼ���$��͂*�i�CM���6����P	С*5���L�A���B$#��|l[�)w�S��3Cc��+�  *2K$�(�Mc�J�f�(�R�}c����6�zo9�#���z*Z��c�^qJ�[��2=���t��?�{5�P�462w�D*�P�$�2Ǖa��c�rK���2t�2�Qld��<fC�`O����6�A���e�4:*F��Q4���(I"�X
E��]��"�x�-����Y
E�ԖB{B�P���A(`�0��P����������acJS��fc��V"d0�eK���0`�f>O�%���:K����(62�L�f�HX� �<�^�Al�d�#A{�%��Ǒ�H ��� ��(,�/M�5�n��-�1�&�>8�ٯa#{�l�o�^�J����L^������E����OA�|H��,ۛE·ȡ�K��7�2#l�Ma�!l	��A���C��*7_�[�5bŵ�ń_rp�(L  ��'��|f `��d(`��(�j~..��
a@#�y"a�H+�����O\,��l��0G*�
���ţ�U��҄����f(O(�	˓'�DW
�P0*{pI'c��l���tG��
[R�`��0�P�PY^,�%�I�t c��|��(�F�_4`���%�iJ�n`	,��L��X(m|I'Cu��U�C���{y-��#�}��2A5V&��ڬ܉h�,��=�1W$]�=ʍUmEa�j������xK,���C�qL����DS(0(k��w��C|��!D����%��� �| ���T��8��ɚLF�ꜫ֗����pq���9\0Tn�w`Z[��~�̠"�ɈA�	G�U}nb5�4D=�(lpýO(@� 	��=�>�C5 �hE'c�&�޺:��f�.�%��p��t���`�[�ϻ4N|Ѕ�d2zTGW�_چJ�L��0�#]��ㄒ�o-�U�F?��G)���M�/�"G��@}5�;�4��
�&�>$X�v)�L��Q�J6R4ձ���P\��Z��PA �@a�	\eBfD���"��P�F>���-(*Bx_�$<��]���#�G���G�T���GՁ�ҡ��<M6���9�.`�D�w�T �
�%���l�ZC)�	�"�G�C/��4���|mn�"|9�  ��֖��ď� ��W"1ht��MyV�!NrMb�\6���
K���Gk@�ȚPF*E��e}�C�4��\Q���O(����ɀ"��t�嵪�70#����>������|+�J#����ڼ�p������ݺ�ԅF�I��>#P��p�E�]g�:#�e��Zk�\Q<6�My*�)��T6�ߓL,H�e"K��8l��:��ݒB�,��$B�4qޖ�L���R��]�Y����&;����%Vp��ILb��Y��R&�z0�g�-i�N�&�x�XLmv%h�‷�(g����vM.�O5���Kq+��ѿ�	!=(�����'���@�);!d'�ϒ�@���+I�@j(����횲:w�� v�϶�$��9���� #�i
KὔGk Ri�+BI���S��Q��8"NxI`�"<�,��!.h���)�Ӏ�oRS�G�������8*	�y�y@�~%�e a��a�;Jy�@nsє�3��AZ��b��x9��C[X���
�G&`�6����@�i�O]T2�8�����}��$���G
7����< � �!�|�H��#� � 8�T���B�D��h6�|�b%ʜC�xޫr������W�|�v�ݶOzUΗh�-# � 5�������oO��$#r�3$��H�.QXI
���=��%�p�����|��Jd�80���\q�ۂ���Zv����e��'C���1�� <��Z�wfT����Dw�l{K��B���?n'��j�$����,�01\f��m'��4�#�h�ȶV\�K"�5��Pd1Uo2�[y-7�^��B��89�nqCš��,�5.?���X/AV���Eu9�若�@�I��H�|�(�>��ce�ڰ�,���5�'�_��ꘞ k��Ǉ@),��	�K�y��E��>�pk�@�[�Y6��D�ے���Zt��Y�˲�1	��ݭ:�I;�XI��%��F ���[IM��j�~SfP.@"<q��Ġ\��G+2=U-@�?	�7��A�W�!ɢ�rȴ�Q�!���5(;��A�x%���8F�����;��qW�5��>�!�!g�?4�� ԅ$֍?��Khp�/	l�g=K'@��J��%vp�H�4��8�%��➒d��cKx����7�N�DhPo֍-�j�� �˾��X�ik��x���0P�IlPKB�R�w��%~�|�Gk �kBI�n%ِ��?���E������H�	qy�X��]i��
���1TZ`��W���|t��P����5�	ܘI$��(N���ҍ֔2(PrROܻ���cQ"Ƞ�"�+����x�C�oo�a /�4N� 
ҒLD/������������n�ބ.~���Ñ���������c���� E#�ۛ%�	�ڰ�-1�ӆ�7� �gI&� �%�Ϫ���5
��Y�P�}��<�c��(@{Y��(@I�q;nJ��� B��m6d'��� �5�D�a���,�dܠ�����(Λ�!�j�� "���  �#7 �cA% WU:h������[a�Z�b�$p�Ƕ��^.i�,�Z��`@}H���Y��Y���� �ˊn�xK ��;0"*�aI'B*`��%2�l(�*����B�T����P���	�0��j�n����R!��k���m�*����E�aJF���8#�� $�ׄ2��D2o�-nUcިUF��	\���`���w`�!�+:
�#٠"6�G��W	D�,	l
��N �l{K��ܑ�i��̒LF	%��P?x�PD:PY��r��k_oB�*ҁ��L'ϣT�,(o[^�TA�'�/��`��
��V)Q		�֍7�P��\J���ϓ�σ���$��@Y�¡Uy��b�m�yB8r���#�mo98�Xz3P��d2
P��H��8W��R#oGdBWy"�%"p�}�Q�MG*#Չ�(�U#�fD3�6�YMp�0T���bZ[��?B�j�+��XEi�7<x`����*O���C �G�`@KZ��(@��R��^���>^ս6����a��p�Ŵ�D�~���J$#�t���%`�*��a���S5�N��7Hp�H,�A�0P�Ђ�A!�((�&�Z�o�4x�&�8�U�R�7U�%�pɣ5�����$������/#�H������P�WW<�%�p!�C5��Wq+:C(/�?�+�]aJۮa�+�#2�;(��\K�}�aA6G*#E���	H��5�X@#��"��F��TiTq�%�X�
��i�T����3��
TeV���<���b����s�a���Dq����*95/d��F^�%�	ؚl{Kp
��f�S���~R\z�=��"^�E�
H�0��H,��#�0`<��JYN���1�V��n@�	GΒ�.����X��қ����$�Q��A6���\�zC27�[�$62_�@)`�)��Ex����8"�(.�����M�Zn]�gj�J�Y����RQ&���C��W�o�!�{�L���Df	,�9�n�R�����R]f��:�!D�,	l.�N��el{K,���i Q)92=RQoRHO�\l.u�FtPK�X��(�`^����x$&7k��2�L�Q�"��&$���x���oo�`��8��-�LF���=ލ����V<����%�M 2D�IHHd
� a�(2E�q'����0),�CӁf�#�9B���&ܪK�l�F2G-M��Q,    X�%��@��N�0���y_>D�TC'k�T��DvπL��}�l�Y�dK��\���$^�ɠ'�"[_��Y�5Ӡ'�������M}��%zi �Ӏ^���d�2L*����-����,d(L8d�D`	DK���}�N�BiA����aŻNE36��'��a�J��a�2��R��t"��҂�y�G�rt�����Z��A�(�pL�$X�H��kBI�Q4�����Zd0&4�P"�#���X�J_��P@�Z�^F ?���1�[����j� �ӂJ �୯O��a^�ZQ��0�Q�5�D WĞ0<v���?4�f���=�2st�}Sֲ���xk�_��ACր>�j����>��2r
�@��$�q���G�u���DuT�F�Il�X"��4�:0#��T�}U�Gy..�J#�%k���� `�	�-�{H�>(.K2=(~�[��Y������iy>��n�A+Z�3h�$�3�	t`���yH��t���rEa!1�n-
�!]O`��@�P�>C'c�����h.aC���T��D~ y7��l����R3r�&�&���#�8���G���<��Ix����T�Њp3�G���I,��L6��d����y31H�� ?�&9�$W�A2�֭�e	��xîMJ3[+*�9	��K�$�/����C_[�8$���7�p&=76� I����7Ԋ�<�4|%�g=+F����i��NS����Á$P
���Ha��P�hf>Kh�52����5l������{�5�,9P0d�D�P���<�\����rO�3�P�)���D���S���Kٖ4��&x�6ωT�)��s�z���R�]�C�������(k���yMa ��;Xr8������mWt2PP�4���0@o��Bܐ��@,�	o�Ld�)v[�������B�%-ynٵ����<b�������\�3�b�,tY�X�-t� �i�}]H�r��FGb#C�BJA��z
��B��Gk@S�u+B��˞��Da5�?�/m�~:�Єn(�t���g�ǄS�̡A��u�s���e�$�Y�H+2��>鏺_�0ó�R�0ƅ�Ld	0��r'f)T:&G+�U1�f�Gy-�x��T6���L,`O&��nG��Bciel�=���y��D� ,�k0��(,Q�;Q������#����\𴀻F��)8B��^Sl�X���!c�d���!�v�C5�=kE'c%)|R���m\�!K�D&@p{)�Xb��؇� �Ied�SqG�8��H@י"�%2p>��A��
���L�3�p��&�么��������aزs��@����::���*�u ���� h�[q*��<�I�p7,�X���bf��c�TF�$�p�ݴ�u�- �B�!��"��ض%�7�r_��pG2�x<}�'n�%�M8}'��R��B3�XJL_�	��V���\�-&�:i0=�146x��)�
x�"Z"��.SÀx*esM)�t�lʪ�;�L�p�&1A��e����̀ �M(�T�|j�(Nқ��-	l$�:���	���v�p�I�����q4-̮=�b��y���Иf��d��r�p�I]��È��ć��ܝ��y,l��Lf�[bW�t|���aI&� ��u�7�UJ��\ ��:�X��J�\���dD��D���^]��@�ي�C8�xBC܉���'Հ!�R�����}K����#2���"��$��0�ZG*�4�(~R~���Ĝ�Il�.�R�w��%�p�ɣ5`���5��$�hV#����3��Q�ILx�^P���N.�3P2ք"x��mu�.72M�_(�՚�G��C<-6�@a��d�$,P8D�񚎁"�P(�"�݋��4l@!fK�C���y<�  ��������%�� �R-��Q4�X�m ��if��	,��߁inx|M��xnP�?	@Y(���Q5����]���P�
�I�v+ZT+ߊ[G�{7�&q'I��Dx���?�����y���r%rg��[w�������D��*�0�������A��Uv2�S�����������~���zxSr9��}᯦C/����oˏ� y,Qqa~��ǲ>X�!��̑4�-�nW"��\�]�z���bmEC�2|��V�2$YYV,C69�f�Mu>����]�,`jUZ:C*�%1�4�9C�8�z��UIh����#�j��Uu��<E%�R�E� ��
��TWD�<�P�~T�U���<r�C�=cYIy����,��z��/e-H�˕���f��'�
_��Hc�����4f��BN?�t�[!qGh����O�j�u�@�a���G"T.��d��:�����\j���s�N-	yBA
U.��R�SH�A�T9/���(|r .KO������;Es�}(���BG����7k[�R�88�L�z2-}Ʒ�WYh�Cu�xE�����1��M�U:��������ۏo�o?������������_�������������8>���o���T|�����ŴME�h��]{�������ݹzo����%�.���Ǝ�[U��)���Ҙ/�'/�㔆O)�N,��G����8'Ő7����Wg��F�T���B�)Y�E�g�h��;:ֽ޺�yK�)�sf쌂�wu���~���>]M�����I����.h���ݍֈ��s�Fi�P���ݶ��Z��:��OUm����[�#p�]1K��W%�f��=}�������*���5��4X�u�tG����)H6��{2�����Z`�{;ׂv�=c/����o}��Px14�{���8��w�~��x����ׯ?����o��������K�Es�^���{GZ�'�SU��)���C܋�$���S,�.D�}����5�o�pڳ�J�{]��vW5dܽx�)�ȏ��;.�-�Gy����!2���0�G�T͞��NK�d^	�a�p�0è��>�<um�!T�AGȕ�(������8E���
i���!v����3�����n]{ܵ\I�	����逓2�e��)|��o�8y�gy�gy�����,�;iA�M�v'�:��0���}��'�-�WWh^]'�y�:B�j�B�eh�K�������lh�W+ ��y�谾�������A3U�&A�F����w�
�{[��bTYH:��;{���uA��i���V��g�*a?+�gz���f�o���4�o��Z�=�!���V���ϕ%L�3��������Q ��FP4n��ڝX���V��v=}_U߿�� ���!1}�;|Do�q44�b��O[NEz��⢌؏�NB����Hb֞�㮃�Z��{��{ȸ��x��ǨR�rB�v��3�J�ܮ��E������#�ݵ�s��>$D�,k�K�����:Wu�^�.d�3\�-��� ��O+|�7d*��X�E�Јo��o�U*�߱�����"-n���_"̄2D{D�F��?����5�T؋T��')|�ݒ�R��2&~��Lro�a�}�~�}�y��_�g��>�{���&� �!������_��L��#-�HX��,��3`�òQ�U�ͺ�G�?l�]���A��w���k��P���)9�����([��� �j�?�5ǅ��� ���D;����FF�`Lp7����$>���(A�L��gGZY5��M������;),�0���h�6�*�}E-���N������a;̥�"��A?M�.�a�!�N��`~푪s	��l�`/�!"�o�ֹ7���UP�\|?�RTGF�N�����hk���9+*���bI6�}G��C��{�?-�H1b����rU�^�UuS�vi��W�F��X@
U�Q2^U4�:Ov��3YU��x�P�rO�@���~q���K���Z�w��b�Hm��H�)[|�k��;���.T��舝{mI��}���������Xf$AMu�s�?wy����θ��3�!�t�w������؇�    gʸi>ҏ6�y;����G�D�w�gx��wJ�8��q`_I������s����~}�RPLq&�b�E���n�����@k�%�nO�&�-�����x _����泣N��'�s�<D(��h�3�d����,:�<B�NA��0�F�~=�/��G[M�H�LvvHv��3�{*j��gE�Q�b�7ڋuv��w�*t(��ȱ�D�'���S�P�w:�h�9W���c<�I���W�v]*i��?��A�5�]3UH���_�3m�;=|yO{�Eu�] F~�Eo�G�����?�������Aچ���2F�r�+=в�#x���;o�\K��ϊ�R��p��?�@�J�*o�V{ߎյ�>	�^�����WV�-#����Zw���GeE�3��>��P���	�L����>�`~��
���l�H����
L��8����:��h��T�f�J3A6�*�
V���n�z{��WԤ���7�O5%��y���:^�إ��np��v\����t�ˣ���?u�}
�'}�/��H�,�α�#R�jE�X��.X�����w�5�W8u��#�2IP�0�p��<Ȼ�@EM[SW�1VGJGX��^�][I/����z�'w����U���u��}=g$1$0c����o�O������.���-���tRPΛ�:���MzN�-+�"�ϴM�?X�*���;��Ӈ�t�$���/���=j���x�}-�ko��E0���j	��l}��ڋ^�;��$u��+�J���;G�9��U�����K�Z߬�з�?ݬӎX����{�pI�n�U9h���JZju�3����5�G�x��W.�=g���.v�KW�7��"&�S��N���|�5t�b��O&r��;���o�F��wT��Thp+��ʾ�3��7�C_��(��1�^5�ug�L�ݫ��,�;�r�d%��I��ص���֫��8	��!rAw_Ӊ��٨��׮���+��+Q*�"�e�^��!�Q6�!6eu�jnk�<�n����7�*�N�����
�pdv�/�5@��;��d�C	y�-�rhf�b������.:Fg-1ܺ\oK}�+KK�VH�
��u�b-�<��nc!b�����?������|l�﫿�w������p<׎}S��sZ;I�7���Ӯ�32Tn���5��\�_�*b���
y���� nܠ�h���C���ۺ�i�m������������(C�P���ӻ3dw�X�7v_�������.N� �8g�����Q.��kB�) �ǿ�2@&�=G\�<BI
�7,�	a�������c�,�q1����Qw�1u3v(��P�[M�+����C��+ӔM��{�o�w���:L�����v��ַ25��p]W�ei&�Aܡ�t�R�\>��� �T6�EN�J�/������\�S��7a;\��d��Eu2���?�����L��� +E�.1Py`��X-gRuޱq?]TF��$�ebu��
�á<���}ˇ4I>���,�f�"�a�v��K�.���>=��5յ�����S0����svR	�����P�-����P�h�?,w]��ͯN���U�""	&��c�>�fs)ʻ2ҟ/(�C������5̑]������d�\)y�tH��=�i[����$rR�^4��Ѳ�uѝH�S���0�_^2uޖ��R��_��S���6ı�����{��R�����ۓ@WS��և�h6�O�R���%p+/��U(��'$�Ϟ/'���z��$N!���̺fϱ�P캯��t	7���d��
}���t��]LӜ�R��f���'I�G�t���c� �;�F(�X�g�p�I�aH6�p�x��m)W}~ޔ?^�=��{��bU�*9gy�uTܪV_��\n��5n�<]~��׎ӽf�zNIJo��V^�P�)6�!��=�d`a�_=	�bU��s�|��s薶�Ǯ��SY�ca���2ܸ.��ޥqљ^-/)t�|�Έ��gL������s��'�wuJ�n�%Ե��'ot���z��O�h��N�����2&Ch�/�����k��lq��x7q�� �^��;i}��r�b�Nw:ܕ�����9uQ����G��*�ˮ��
IU��D�5�-�]W�Fk
��ģ~�0�nZ�s�4�|^`Uk(�$Sa_�6�t��ġ����ۈn��Ε�3�ĕ����(�>�"y߷���si��#��*IΞؾ9T���Í��T��{�Q��&��x��ϒEE�=�������#�� �L������;��9pQﾐ�}�w$slҬ�g �c������ �О��`]/�&@�}þ����"܎�3��]8�'��V��t��Y����O��H[O��7�I�yI�&)�\�L&=��&��� i���/��{�$|t
?��R�f���:�f=*e�`�:��9xS����|9��~?^�������⯿������ǵ��e3zE�������χ����u^ ���l��N�ѰF�^��;2���	�Ki
�B�ŕtəå����$�φ{�l�<Y�{�t�_4��!�J�ώ:���z�yUw�J�K%ٺ{RҰ���oS����⎶>7�cI����Ô��0��֏J�f&�׸3Y<��7N�9X]�����6����s��,����q���Q�C�l�����]�x���u�ɅY��G�����TS|���/�Q�H�fsT��{�2Ej�d���Rd��`���>=��nw��c�P���`�zo,쥼w���x���D�?\��]� oUݝ5�1��O�":���[Z��"o��\���tA���5�����K��]����!�������!��Vt�%mM0�p�M��t@F��L���ww��}_�p�z	pp�z�ž,M�F�uHoL�� �9�1�kW�8�k���p�FFS��gq�����y
V����}�î�����)��NSu����ݩ;���u��E*��G���?��lP�f�78O^���,���`���==��=}�*�9FY�(N_b�o�eםL[�Y�}��3)ۦ��^vV��*����E���?�C���g������I$H�%����-�|v�UзO3H��s�xmgʖː��dTN����$���|!%;�x��=	2'�ގ=��zc��3=�?o��֑��Ԕ��#e	��d9���FS���i���*lY��Hȷ]��g9�'[���AA�@��|6f����3@�v��)	f=�͉�����4QsP��-�B������k>�6lw*�?@�?B�4b��NT�/�����{���}#��։�|[�0���z�X+c_M	��������Sv��V�r8��m�ݒzٵ��5�i�Ο�c��B+^^2�)JFѹ�l�+Au2q��5���k��o��g��nk[>'��ߟ��p�aq�����WK��Vp'*e�R+Fs<$���t���L�D�ø��{b���t2�몮vi�x����������cU!���Ȭ��h���T�j��Ij�99\&�����MVy�����Kw别*�Ǩ��A�*.��]:����28��v�3���>/ _W�a��-nݏ��Q�����.?U�<=������LA�b����܍�ޚ<�+w�L�XH���F�ۮؘO<=����p��I8t��:�*�����:tNcF��ɂG�]�w�U���pJ�[��S���o�u�w�×6�j|w�|��X�.�mȆ��B��0���-?�KG/嵱���ޝ?߻��T�4�r� ����?�]�g���E��I;�ʖ�N���nh��*f;bdKR�#��������'��~5��u��N��oD8�'^]V�e5����,�W�b�������\�N�֢v�$��?u�@��][,����6\]B���m�z_�ty/Ǡơ�)������zl�=���W*kuD'T�B<=�OȚ���V�?ջҪ�%)��َ0�F�[u��}/�'���}�� �n��ջ��-�zaFJ0)�C��T�\'\��F_;IS�H    S�����Vɮ��-���sqP9:��Ọ��Jv�w�\W�zJ1 _ ��N����P��ܫB��b��_'�gLi�=y$���%=��A��/�$�lH�0z�J�٥'��+"�S5�%�O��󜫠�c�`6�*�		_���-^�џ�r��T��}�F�F<2�=�*���u��x�)�v�Y"!}�-}�����c��Gbb�������5��&�݉K&	�Rr�����V�Z�D9���9�l���'��1��,+�z��(N8��hNsH�g�84�lU�֤_�Ulߜ	�t�(�'�ey������N�\B*͘vY��O�$c��B�k�e]@�t���5��d��]wE}���݃�.��xpF*��m=Vd��t���r��������ת~�ڀ���7U�V��"�fP��V��Ի�ՙ����9���ȫ��놶qQZ��$Q
#��ķ���]ig�5����Z���^G�|��%j��O�tV��v�0n� 1+��jPO2ot@�,��o�V�DHߡo5���q�߹��[^���V|is߰���S��ۢ�)��"�꡼�IL�~L�Ju���*u��G��Q�T��8��Xu��R��"��ŝ��QH=��g8v�uq{z1�/���t�n��!h�j;��:��k��ɉܻz�RN����� �FϷ��i#���s �z���Ȫ��O>}�}�]ϧ�|�V5m5H���<Z82DL��d������qm8����.A1$d��}�ew*�#�+�o#���<t��r�d#�}�}-T�G�^�;�f:�{�C���)�? ����!����|���=ńTz�9fR��!l�6��:��ڽ�}gj�EF�)T��|�"���J�ڎ��U��4����8�&�s;��'(t�s�u�h����%#��ncB+(W���:��=�j��|q�=��z�=��O�1��3�]IMkd�X�E�N�����L��)[]�������9
�t��S��=�rK�?��Vm�vf�v���O�rN
������b��䄼x>��Cz%�o���TX@�mJ�O<��DJ�}fq��j
|�!k*�cy�;��5鍬�vS������B�mB�����n_�)<@][-��d(:��+�î�������ѐ�u~I��Ѹ��	C��) ��x
`�TJu���WC�\�ὁ��CZ�tF�����g����Cڶ�K�&j2T��T~ �=Kt��p� �3��S��q�˿�Ov�+'Qz�sƚ}E�h�5���x���41�t��\��}�Π��Šr�O�b��g�=W:�]ӷcҖ5��E9w�]}��,-2N)Ph��C==A�=���3d
ʌN�B�'�<��KP��+չ��z�\�@	x@��ۜ]�!]���D&Z:�h*���\a�������!m�A��υv��Z4�qI�I@����ud��Q�/�թ;w��[����L���	������/�+ �T&E���9�Eu��G s �gs<�4j���(��{��_��}���T��X�;�<q�N;s}#����{q*���� 2�C/���_�|yT��;zNC!R��B�*���ظ���.��1rh"�hN�CS Ѯ��V��ѷ�*4%�\渤��.:B����k)�Q�P�5|���_����u@-q�b���.��yT�X�`!'QM��4���Y�P����G ���n�֨�`�d��	�txV�sj_�\R���wʿ=d��cB�C�w>E����S]�}�p�m�G&?��p�>���`&�}�p�N7���u���qT��uq���oȬ2wK
T��F�c��z~ÒyoQ�����ŭ�h@������:]_�f��ǫO�/���q�������kRz�Fs&�S�e! ���R�/K�h�7Xm�����#/�u�����G��x�T��2T,���}�DfB����ە�i��%6���Eng�-�k��-�_�F���.�ƫ!��^�LW7�o���qk�:����g�+�E�Gd!�����]S�~�{�Ӭ�Cd!������f8��\|���y΀:��$��7:=�j���xQ֨:��?O�7a�{����u����Os���\ˍT����I��VQ�_�G*E��H������Y���r_|y/5^Ї������ȿ�2�7����b���E�nqK��-&wo�Qw]�yhG��$��[M���P��43R�ٲ�*h?p�!wt�^�Eo��G
�쨥~��3v{��>�T�
��/�i8��Ei�!'��ȵ��?P�rҫH���vW�>x>��kL�D':�˦�l��;�kW��6rD���-���7�#�l�hR�@����I�����?��?ؔ���<��جi�쮪Wｶ��O!��N?Ej����\��]w�-%hS��m���Ӟ�k�E�(�\��9|Y�Q�x"�S.����7q��fMm@����;zFW���/ز��5���Z[aL���F�c�����2�?֗�I9�j��&��׽w��KlG����#�q���y޺���~��=� �ij��]=ѻe�Ot��[����?�	�j�Ǩ��i#����6����=�s��9�q��L�7��e��yU�H��~�@;B���]Y�������@'
W�4Lr�e �9bھ	v2�*$�t(e�·ȌGR����t�89�J�#6
�X
�@��u��VS�	_8�2pE7aX�+�oڍV*��d�	�BBeZb�������E�kc8)���R�_��a�맍�-��W�ր��qvD+���NM��2%��I1	w��̚�ثܐ��1�,B_�̧�U��"��ʓ��ʐ�LA��q�R�G��AK���4��bE��\��y��J�#�ACoS�X9���k�q�Ja#��G�uiVw]��W��u�����ƭ�ǡ�yQ%�W彌x�ѳWw�d�I�i�k���C:|���#]�t_)_�A�.��Qݮ�:�L�'+MG]*�G9E(4���h+�c�z/����*����aȷ����᧱��gjN�b�ˏ��)����ob�ܽS�U���籦u��d�ؒ�)Pm&�ޥ��|ե>��-�R
��� ޡ���u
_dn��B)�)Y�,��c�Ư��	�#B˳�u[����ߍz�2�T2�]\�ap��Xo|`�z�
��G�v���5�p�:�����S�%�*���J�,��,� �V�e]�	��1�"�#W�FS�ɅK1��3�R$�J���wQD�ڼ�������O�n��}Z�V��QZ�$'�q�"/ �݂�}�2$ٵ�f�Rz�N�h��b������e�-�I�M���X��++u�+^�%U��PzfEB�v{Ѭ����¾D���`���8��ƿ�s�o�;��w�������� 6�(lT�Ξ��3w݊��;�=�p�9ug���&�#K�y�*�D��JC[^G�U�����49�A&��s��|�qҳbTQ���1:��0���wE�u�C�b��7J��
|xI���w�V:�U���p���	R*P��R�R�#__ʾ��|
Q��tG�}fY]f�j�<���ͧ�w���˷?�'U\�u���ƌ��{��Ѓ�+�Ѕ8�޲g|aK�5J�d=� �"3���J\|��_�Kވ��'+"���hW�V1�%�0"D��(�H��]�>������o�"��b��ƏN�0O�>�U�Pq��%�5�:�Q���_���.�#V�Y�f�D�(T
^\�X��`;p?��k�;�L�ћ[\��iO��Hǣ@;z�))4@��/�Z��h���,�Pa2�F����|���?<�ֲl���Nr�a�q�p�3�(�ŝs�AB�0��_��Յ�%��C�Éy!����G�K�V[����:�oE��[�eI��4��
~O`�c,|Pi; �(|���F#��!�`g�����?�Ӟ>lt_���>����o����8�S�� �����e�Y���	�Y"��V?''K��81|{���sO���9�p�d���>6孾NWtl'%|70����i'�Tb�{��aϊR�L �  �Nr�<� �g󗒃�v���;?<٢��x(��J��G��m�=�~^�{����i<D^�9�r/�ʛVA�Ien�"Ղ�d2���~~�����⢃���i�e�'����2/��h`�E�����s�	����0�q�O���r��A��~~�z�0�z�
J��d�T�+��}�)b�������p��R7my�F���J �g

�b�cm�1?�ř�T�����A�4q��}ēc��#��Լ�3�l���p�qEhІ��~��,VF��Y�V/%=[��t�:aO~�~R59�ʐ�BB�҆NYZOg��B�`Y���	�'&��Yʮ��_��1�J����f�mΐ�m���W�H���x���׍Fʹ��t�Z�]����ҩ�b
|]��>y�F���s�ӛ�ݐ��C{��Kz�5���GU�0M>�C*�_8�J����sn�B^���/Ss�
��:e�#=������~�S+jL��J*��EU�������{9��"����V�҇����jC�O�n�jM/@�������B�W�,up�8xa��I�nK,�G�㘬1�v;�<V_��7X�:5� �� �L�dsݾu�Vπ\�	����Z�+@�b��x���%]�8��D6��t�km��(bH���]�3��̺�S�ق�G�%�pGOP��/���� ��c�)��Jwh�����;��"��s/��"����Ωl�l��on�߿���D���>�-����T�O�1��g��S��9j�fS�I�%g�A�SJ��oQh�i��J��
ĶD��vl��
1���%���X��%�#�����|�� �%1���-)O� ����^�17?}�V�g��
�Ԩ����L�:uuD�m���_�>�h*�ɷ^�ݮ�R�Cs[��&yZ�(�{j��֌��;��R
�/^P8�g�-�� �|�����Iq�>���q�%�J=S8v1��4��:���8�����.���ǡ��O
u�p[��o�*�w?=޻h����c1q	�{����6��E#���g��ٖ ���e�?�Yc��Q�������!V��-�_t�R�ҙc�K�U�~wtū�j�q�;/�� ؋����,wB�8��;��7�K�ٙ�F�i6��;Q���SՏq!��C��-�*��a�/�C��8�2{B(�U]貪�S�<�2
�r/�H��T.�ie�v���Dh<縘�{R+�6��\;���P�TK���z5��!�ܙ���g�a�Sӻg==+D�smA3f����ӗ�^�P��a��i�[)f���U�6R�Fv�Y4�3�l��/�sY�,l/&�v������-q��o�cǐ�B1�]����"�d#]��W�����x[60�'N/=�f4�*���~���ʡ<]�ޖ��d��T��������̮ı;�f+%���AՐIy�J7Km҄���/K*�D��8��|׼��/.t��	0��Ɍ��������d�9@��9W+�����^�}�gC�^@�}�ڊ���)U�KdJ�����&�8�,�r���)es�o	/���ԂVJ�:<
��	�6cG�JpM��!�Gu����R)�K���!�>U7w��#�g���ͳ$�<���!
��?ZF��,����¶��@M�J9��1���;�i�z2�Uy��$���:>�Ŧ������wz�#���2�,��
�|c&n��s�
�*.���X�h�Y�p#CxZ���+^�� z�j�T�x"�[сvW�����v)=�t����Q� �(KN�~׮��Ur_�&�a�	��82�Q�}�g�I�\�W>�z��P���D��Ƭ+�NE[�Í`�	S|R��2��W6nń"t���<���o�s���Ks֩&����,ON��S�����Ar��,�Q�V�.Rޓ���t��nP�f��Z������!����BV�*Mۓ��2�  Suݧ��#g_'L8����Ç�}���      �   �   x��ϻ
1�:��,������(bcs�A㚰d�¿7*>����a�Q�\�-|�Ñ�j�5Ǆ�Z�9�N������QW�9���0��m|�4~�-�|�)q�^��"����#s(�i9 e�9|nkx��-jw�?�����8�+6��;�/uUY�ѕ%��<�e��u��]�b���.v������.� a]�"      �      x�u\Y��ʒ��U��-��S<T�B �;v�z��h�G�s�
�UR������6܆.����*�VA��N��J�?��5{��̏�����y����X���q�������ܝ���F� �Vڛ���_�8\����^�x��{�K뿺��_v���W۶���a>w�e�d��ǆp�{#��;�/t]��-Y��F�Kw��j��!��F��Wߍ:�����5�[�.��0���g�`eԾ���ޘ���iw����k��1&�`R{�O��7�ʗ/�|���V�ڽqU��<17Jj]��X*�<��AGN�Vڇ�	U�{���
��i�bU��<y��lP�%�VTٯn�<f�|�y�W�no����8�=��p�%���]�n}���f[ ����֝�+�U�!����L76��W�Vx��[�~�Y��w�5�jo��3c�`�f��v,���agL�^�_:�L���s�/C���~xvc�V�	V���ʩ���a��)u�����|͇ۍ�
+�=�����;�����v&J�׷]�,���szuw<�-�v����p�O���a�t���w`7{��u���z�G���q�
����	��-v�]Q9L�[�:]֗[��E��^a&������{�1���v��؝��U�akK!��
ώ�d.?!��VJ[ů�oPQ�M�~���������1��8��%�n�5͖cH�M�`Zt��XR�*|�������ҰI��20�S��|�T��6ޡ�1���zeaƲ}\���T���_�c��el�(�^_5h'g*���7"̮�R.��hD�\�c��"GC��r�j�>)͑�)�,�'��W���5�|ފ+��ֶ�_�-/�"�:�!aX��i�v��4_���WZ��$�ٴ߹�.�S����F���	3�d��ېv���6j%Ͼz��_:Kedp��}��)/��i`�1*ظ��p�6l�$}�04H�5,�j<���Y�~�@�Nk=\�/��jӡ��U%l�rkD��b��vL��fZ�lI����.}c�
#KW�fR��
��	�=]�|��~˧��yZ����jo�a.I�㪴"��e�v�A#FX�� ���Ǚ}�V�\�xr7b�18��ڨ�-/�?�8&�[�����l�U+q,#L�#����_Rq�7U�͍�U�2<����W>{��;��ݢ����B�콏"/�V�|U���u�����P����sakY�[c�z�1�/3�:^�����kO��#���A�MAGj��U|�fL���L ���>����y:�cL�u��"�C������a�?�#�S�:ݵx\��y���u�[�B�k�}�Î󣻖ψ(+h��`�[ �v߰[O���m��:�V�`m>	'H��&�H��jw���L	i�@~B�����}"���'�Zi�)UkK��"~��=�1Z6��R��KS��$����UK|��f��'��h: <ڙ�\��0~ Ψ�4͜+�����-��4�b�]1��k��|Ml=���G2t��	��Kx�2���{S��?�p���ǜz[���$IQ���l�/Δ&�vJFK�4�;U�����J�2��ՠ�K:�w��'ɵi��)�,؀��J
�%�4��B8lV����hݤ�?�o
+rᳱ�l�[�(�;w��K��G
�[^��է�;�r��DX��O��9��ӱv���Y���j����%�<6b�Ē�Wq,��+�^�Y�� CU�%����	����8ZoD1ГF�LSKI	E��5W�`L�mn��y^W��p�&�J�6a�	`�*�ݏ}�����p��{P��|9w����2�+#���$%���@N�7Y�˘ƶW��kk2�c�:�c��u$7>Uap�8�ɨYi&��dD��s �z�9�റ����Ĭp�:�u9���o-r@�u��F�n�^K��%�� �z�|6�T}6��w�P)@���O�f������XBС0$���0�o�KO����L�u�R���o����!?0����**�&ߓ�X�Rz�n��
��3M�WG�괍e6Ԋ��=�\��lM�1LF��+�u��Н; ��@)h��0B����1E��%H`����k����Z�;�`,�J�0 :���O���`�*^�b��m}K6Y�����]��/���8+���Uvj�ɗ�Y���(��Qր�U��|�Y�j��@�&ܢp���%�i.�2�w.�ӻM�r���W�Fy*�E�ٟ��C��Vs� _�Y�w
�9F��:�a���%0q���M�lv�����_f ͧh*X(X��>A![����R+�����јb:�@��<������b.Rh�Rp|AX�6m�}�	e��Mr��p/_�)���r:/�5��t8�Vy��tF���t8���V�c`�f!�����./� ��4�&Y�fƽxk$��6	USm����x f�F��퓂z+T�E�hT�'���\���m�D�l
���`���.�0�fK
ge	��"�sx�AdV�D&�,�;����<S' �ԅ�m��	�h�&�Q��ǵ�� �.$r8�+�T�8��gsL��E�!�36` ��89�9�ƹIuL-���^�G	���&$��v_�J���%to9$�0��J�|q5��8��=�V,ĶgxΨ�5���U���+���+i�m��0�y�u�l*�<�-R�³�8 ѵK�T�b��Ѕ��ʓ��M;�U�ڬ48�VU��@/���ǃ�	���ݧ۫�ޱ��}���g��~Fi`�؇�@/��p��,땊tԱ:���<�vi��z���x�qZ�׹}�IK�w�J�S�h��c��
Ja�6u��ȁ�
�{L:`?�b�6jX>~���Е���B,l�� �_�y�c�y#�g]#�����c�c π!o: ���x�y�iu�%ߒW�.���e�L����[y�?����4u�.г�<S;�i���ۤ�4�w��>Lc2�ܒ�Y+Ʀ�֪򷻒�RVB�|��WU[��2g@V���r��L0*�m�ťnrJ<���+�
����B1���/�&�qQxΏ���)�N��0��-��^�7��hJ����5�`U8�.X����+ؤ����'�J �V8�p��������8�i�����;�aG"��)�����vHo���9}k�0p�}`݊�hP"��8C�zH�H�$m6Ceu�~N)�Fg\웬6H�[�Q��qu�n�&��pc#1���O9�R���D� ���9�'/dN?�1�����lL��kG���(X��R%���*\��p���B�i^+  �W�����rB�	[i� ��;MG��h�Զl��zηWG���pA2��Q�*I�4�}�	�������6/U\.ӽ����6,�K\^��bH�9m�
|C�P�.sb��1(�� 1�%��w��n��6�Ą
��G��	�s��]���HZ}dah�I,T����#��jG�On;��PR�f��0^�|�p%b]٥+q�Tm��m�#�tBK��[n�<���)ǃ`�l������c�c  *S���X(�E���m-y�O���p͡P#�Z��(���<�4(�R�W�F}������(���:��a��{���ʳ�(4�׼'�����aHUiظ�5��#�=�g��vA7�S�0�[������ٖ���|J��RZ�]2+-8rL�Z���b����e<�C;�D��,Sd6cZ�t����K�����S	ޗ�����E:E�]=��9�}姫�':�[��?�;9 �)��fS�`l��9t)L)0�3�߂�9���n�aia�L��)��N�������Z�XJ7��8_z2��{�_�K)�Z�w��l�gZ��3I�F�W��>�ZIկt����:��	T�񵵔w-"[VYq��dn0���RVW�7`6��G ZSr4k��`~�"-z�z_6��E�g��tC�9g.��g�C*�gD"�]/����	ޤ~    �(�O�(5)SF��0,cI����1UdIh����*��-P�n5V=����1�:�iJ��JO�.%%���0nE~�I�a=�y��jx7�	m�q�l@�M`&�,�>h� ��V͍.���څNă$����@�X<����1�޻W��Z���(`D�=O�?������7am)d�8�*�S��*�ꛂ��җ�bz0���L���e��s��!æ0^���;u2���F��¶��a�e����&/Y?���h<��G*�J�Y�
����!�W��q��%h!+�K����,�0��|���B��(�
L�[��>��b�-�[Kӛ8�F�V� �*�͠�udq؞,����Ȗ�rC���E)}�2��Qҵ��Իy�с����Wr���;���+qZ�M,�jC~C�V�aE�ָ���-���nL�LM��u�:����|t�ً��׉D���q ^Ү���U��Uᚂ�ށ�Ȗ8Æ��U<EB0����vd���nS:�`�$�=�L.�,g����ʰ�5U�D�p-ï�&)����n%)�h�_́�t��u�0�������_��^J��N n��K�c݋h�f�8��!=��o�M�PpE�g�:�ˢ!����L��҂J�LE	���\{Q�PK����4�D�[yV��g�@+/ش�ɉңM�L�CȪ1ͅ�%@��hQ�Zo-"�A<֮-@q���n�3��|3D|J�4R�[X��9�٧ZE!m)Gj$*�w�p|�@$�s��N�$�*��Ü�f�\�\�ACZg��u%������M�_�Dˍq�Ӝcq�$@�>�4Y�]����'��:�>a�k�P���ki�"0@3V�d���f���ϫ�wD� Զ
e��
��!%���%��� ���q�U J��A����r�'�lcr�Aa�7Y ��WUaN'5Hh����B�^���ۣg�T#���@Z{�z���{N'��>Сu�$Ϊ&��aZ��t��ɵn�m�.�/�NO�tOyP�U�z琖��\�sw>|h��:X���Â\�R^h��H��$v������%4�Sl�~�#3D+�n���.���2�2�|S��(�,L�Ǆfʁ2�l[Z*!�t�c`8H�_�c��'i�f��j�Z�x����&(��ʋq�H�-��,�H ���� ��#P�)[��l�V���R����3㞬P,�yE�r������ *���y�Z~`�A��Y�����=�9k���([M�c*>s=��#G�eam�{�?���th�H���T?��EN�/��著<rH��x�-.�����4�M�qX��]l6��CL���&9˜dC'B'|�|M��zL/o~ш@p-�t�ح���HZ�U�{��w
Yd���p����Sho�����z�	��:c�8�2E�m@�T�+�/����#��Uh�J��bQ��x���m���V��4\r���<�&X��[��"~�Ky��V 7�{�����a�mt�H+'�}�O)��Z����U�0c0-|��U��2�
��L#�֖��ӝ�U�<O
�Z��
ҫz��$w`�_���K�s>2
:΄@��:��]���� Ю��4B�Z��KB2�Ď1�:� l+LU8w2�6WS�rR�.�s�5 �+��[�$�Uq@�۔"3�7��8�{�x7��N<>��,��,FK�u�,��A�z^a�]y��y��H+�6
y�Gq- ?	.�S�x��j�Q�;�s���dK��Ό�5ܙ�,�1� ,)�y�l�1�y�!��ڜ�T�WN�X��:�Z6�e�6,���RF����~���]Tf�����Պ��K����mNg}-���$q]ű�S) 96@	Rok}�Y�q�G�e�.�^��t&�S�����%[�K��eȉ����mZ�Ń���8u �Q�~c�e`����s�"�������`�ۥ��8��b����#z�3H����]�Ă�`����9�Zp�6�Q�v �Z��W�	B8�#�]��c��)��	�V�|��;m�k��UFU����x�Ə��!7F+y����
���ȳ��-:Y�Y~�b|v�ИzRk�"�������	�z>s������)�*�DDC<�!��Ʀ~��,�~[dc�x3�����Wh�U��7%�X_��K��Y��Ucтz�\y�59E�UK)V�ECG�f ��w/��*m�gzp>̗?�2=*�b���V����pJ�T ��t�]	���RN�1i�y�F���J�S��� a|+����fQ���\ƥ�oi�w^����/� w2���O�"�	�b��|˃4��[^gs�6̛�a����6��I�gx�ᙂpK���Ґ���H4�*�~2���Y��4X���m;x�A�w4��<^D�c�0kL�]��9�^����۟��B�5nbeZ,����E���K���Ԍ}_�P~�/�1��e+�,�_�7��b)hpʋg�}�M�|j+ۮI�m%���tԉaC�Ӓ�^/0��pM�K�<�|��N��}����>�2��ܡ�dN��s��wd�MA���.T���d�5�j]g��Pes�z�DGMTe������e&�;��^�l,�\�͔4���A��,G_i8]�������嵑�Xk�c\�9���x߬ΤB:�`0?��u.�I#�,�l7ާTKk]�C�ۦXQ+ ���j�E>[��X��[Y������/�4�
�Y�ES@�ZXB�V�t��O�R���������-0"��+H�о��jq�r��c����x�Z߱��៎�����nմ<_dݏJ�[������i�݈l�'�1�	\�Gx!o�!g���hW�26o?�K�[�1g�>Eɂ���ε٥V��d:�^ޠZ> ��,;a't���{X�{a�����I�t���V&m��&�����s9� q�,��]榜�����.�K�{��grG�ҋ�
U�N�#%X50�k�c�b�g�9%����Aˋ8���Gr��,��t+X�a���= �6Y�RF��}M@I�k4���R�e�ţ��a.����R�N���mM�l��\.E��Z��b�)�l5#K˨L/��)Vb�l�q�"�[L���5(4�˜�(�%�S޽֘���-|J7DE˲�Fc�q���G�`�}��v��2���y��`QqI�l��	��O�F����]��|��r<��$�Z�M2o��W���=�R���7!.�H�W��p���7ҼGhJ�T�]�q�|B��\fոb�X��j9���8ɬ�w:I�)Q���z��k=�y���8ж`�͹>i���K!�s��n5��\\?ܦKʘ����. ��K���K<<#�e�6�R��K�y��i[�,�u]��`��b�-�`1D�Ub]�zWB`A̼�L�P-lK'�����Tü�,'
ZG���!���צܙ�8
�Pֱ���t(����P���K����GY �x{I>�zI`������W���0��͕/��e��&�R�=��v�fl��dx}�_"��
ط��D����J��,��'ny7���.|O�����zF��G�@wJ�X`��/W{�(q|��FQ";kyt6��C50=SR��妮U�ZA�|�;�.) [��+֛0�\�Ѝ)��>�<�ÐS�E�����F���^�y�nK�GDݬ�P0�.;ޜ����垆��,�S
�R�d!�.�ج�)a!� �s���3Y�@�Q�f�5�c)��z�kS�������C�#c����	�h���1�/��NK�ܯ��E!]�zI�æ�%�a�ڙ�|��7Ƹ��>��Y���(�藵�9o��]*q��L�ZY6�B�궭�7��ַ�y~ӗ]׸aL��RS�l�Ę��H����Y��7X��!i6$��e��ȃ+N�LNˀp��������K0�:�_G0�Ѿ��o�J�KF�1~���Ӆ�<�Q����`�C�HG�%vm����r�|*�� hla܈'/ز���^Z�z�}��w�}�ȃ���N��,�	4�EZ᭑�U��N��j���_/�ފ�E��3Ӊ¦+� &  ���+���xt<b�:�oJ�+��PGeu�`��Zf<�u��z	����C�JzOL��=���9�&9%[�lʳl���ؾ���p�5ozCw}�h�{��G��ҤMy��ґץ��Y�K5�w�2-޷������wR���&NI�d�G㵎���:S�ǫ�{��IG�kϚW%1U���<Mwu��(B�ϻ0=�k�o�bi����l+~�r-�tH�2W�q�;�ӏ�O�R��ҵMA�?�P�o��r����P�
�I�����|H�B:�R9�6�3���������      �      x��}ۮ�8��s�W�A?g�.�/�tY�VJ��c�r(4
��L� ��CJ���A*t��Ė��k0\�߾�Y�g�O.��eU~�������ϯ��������?�d�G�=���o�����o�j�W�>��^�X��K4��$�v����=������/?���?�Ke�������p���_���?�?Uk�,;�^a��4N�Bw,������g�g�3uM��)U#������1����o����`U��O�V]6���|�G��L3n@�lθ ��\ �5u��w�ꋣE�3�=����N]��^�S��p�9�/������n�N�3�r�^`t�~�$k�$��uz.pY��>��@6�,�r�XՔ"Tt�kHVڤ�ԌSr9�3�lûI��%}o�c#�P�*��g���c�U�+�4�V�>��in�y:�࠱������mJV�
�V6�a
�a�#�����%W6#����^EX�l��[�+_��ޏ���*�w�[����#N���kjS�nY/J�~k�3�0�I��q��o�r�k�ݱ�v�{� �{J.�p����X����;�t��p�Qg�+��X;ѱ�;�a�����Ϧ��/�cuw��io2���ϩg����vX��f�:�L�{��nҕXߝ���Mމ�>��4�S�%Vxg¢��'z]Y���lN1������<#l�wR߼�~��+K��;�֤�;~��~MW"M��E��d���7�)�ʍ������nr6��lp�{�o�W��:Rp�2�C�r�G���{��z�Kra��!@�(X�[e5��1�g-S�d���Gv�ni�;	D�����m�1䋼�o�s�4�cd19ǀ[y�x5>:u&�B)O��m2��򽖝ޮl�.�/�p����1�&'��M��h�G���2ln�{�����a�_��o���c��|�~X��y�m��=���Sv&��;
^��K_j&n�;�"TZK橊��&�W�"pG�x�yG���쨸w��E��K��OF��]ͫ�%��d�Vz��*�ս�Ta�)'lU�y���u�j.�k�*pBQ��;.o�ʮ�ɩ�
�����2O�w�׿Py\we0�N��>�
%�=w";����'��U���פ���UX�����j�V7���h��s���;%�����6�3e.oK��^˖�`=����LLu�w����_Ұ�i�2o&�(�2<F��˺�ga;�٦��p��s���b�g{�盲#Cm������(�k�N�c8�]�	M��{y���_>R���<�p���m�p��xFNn�=`�Jd���'��Z;�]:˶=��<\���ϡ������	Olҗ���8�lpo�7��ߒw\��[>��Ixg���q�isO�������O@�X��#1��������@����O����>ݔy�ntB/���`�J���#VA�11��=����R+K�%?���:ރb�Sa��\����2So����9b�S�����_���ou��X�T"x&�v�x���~�c}SU�a���mc�ړ�-��oq�k�C��\�1�{���Z;�Sp�w"�'5��r7}�w�y2GpܗQl�u�Z��q՞��y�d��/Ǚ�݀/_�K6�Q�C��o�������a���c��6��2�v�c�v |�6�8�8��ݳ8NX����<��u���lp�tª�p���=�鯆��~9=a�v�|�K��&Wɒ	�7Vjǌp.9Gk�����3�}Wc�v&�.�(����d���Q��Dg�ψ=��<�ʸ`������>#�n.���k�]og9��c=��3�����H��9a5���#����.Mr��k���%k�����7׏�3>�+Su�:���nvz�Q��<�N2G`�?&���#F%l�%�跼O?$��C��Qg,6���E��л��"�&�5�r6�VT)+�z �&��><�����b�q&�<��P�,
��ȶ��+A$��2��S��=�5ê�@0[m�!��ʞޙa�q$\�6wbt�}sk؃��k��_pهȷ��N�i���[�J��'v/m��c8yl�K��
��Ł>�K96���w\
H�xj):�j�' b�&�#n�}ෛ9�q�G·j"���7� v-�r�±o!"%��p
��k�U͑:PY|��{^�͞l|�QryݘS�	��n�� ���ַ���I19���Gҹ=�!�c�5܊����]��"��ne�x�CF�KU}n��7�=�ۆ�60�<?��������1�<?�������W}����~�cy~&�S�sj|�7��^�*�y�EFxW��#5�.��	i�Ss���1�����9�������"#�6�8��Z�6�ɲ�s��.2��a�����k��/5��M�3iH�9A
��b�e��5�|N/��n�ñ��JQ7!D��flZn��S���8$t�G^;�h�W�ئ�̣u�u��-ݳar��.����)��b�MeoJ��@�c�vQ�W��Z�T�f��� Vu� g��c��k�Ɇ��U]Y���;����{�a�vQ۵%縪��QvL5�'<�ogǜ�`�����M�4��0�;;n�w{6q$s��ن]�G��e3�����U��eӪL�Q6Iʾ�7ayv<�'��)��f�^��u�n� ?����#��u�G0NUƞ�`}ǔ��H1mm�Q�{�_�Z����bbyv"���ܣ�L�������`����y�ߖy������D��u{RCO~M;�:�:'b���G�?uW��u���N��Dܼ��G�P��c�}�}΄�w�G�r������ۯ\�A3��t�)�u���?��Gi�R�s�  ��f�����g�cg�bjo`�i���}&�us�Q�ַ�n���܀p}&�[F�O�?�g���~ū��9n�%�8��!4�}��=�^�`7m���su�~�}E�з���:��6�Kbm�.R�y�S
��l��f�g\ᔷyǰ��b��W��'�e���EW#~�D8��t���d�<��0�x�g��=���]�G�O�Q��gP���w�3
�Yp�G\J�1�������5ӯ��Xs�aԷ)��w[sj8�u*�,ӯ�����<=����:I`�����*'.�ج#�ߚG���~����<'��\������=̵�s�q��֮��^�u��5ix�儗d�W�P�xk]m��.�s� (�>�z��䷚�f�o6��>�Z�Ax��GJ�nC��ix�1�L� ؟.�X�k:p�ky���9\A�{�YG�����v���gz/	�fA�k��c�U�+7L��K�y����H�<��$��6��L��p��brw^g#6��ZӾ��7������yI�N��#��w�6����U�v�%]�gk�ZH15�r��A���GЇnx4#{�yx̴�qC��6����y�w�a�u.���;f������sA�y[N���Sm��U�`0�0�Z�{S.��n��/&�&:p�c��8gB��#%蛮�5>`L�\�/��k��#F&��i��M��-�%�=�|���U�.��:/���s�z+F��3F&���ͨ%�u{{3y��Z-��CVr�o~aV�7;fW�#A.��G���̹���fW�#��Y�*���~nß�2��jq$lE�u���`)�kq$,F�y]���G=&Z�������N>��V��=�+�3���̣��T���'L�g��q��uWf�䎙�+�R̿g����ůg��]�$k>��Xߝ	f�{B�5�I�1���gʎY�N(����>8d�uޙ8�t�������^��.�Ĺ��;�����7�\`Z�8��k��|�vL{��15�ʨx�6�(��y����� 2#&j�3q�{��O� N��̸���aE��q�j�b�ͣn��*��^�*�b�̣���M?2�}�������d3��K}J`�k��g	7��D�#�{���G����cx��f�u�t^�`n�8P���.�<�w�]�<n�    �sj�6t��|��y�|�x���{�	0�Q�����lq�o�(��빹���6�v�q��ms$�s�X=����0XU1/[	ϯ�<�>6C7�l<��j����ő�y2��k��c��☟-����g��j�9B��L���'�r��w4z仚k�o����X�̆gč������}���WL���O@�������&KAݫ�5I�E`�l��&���D�����C�$� �l9x��e�e�=� �6�o$�m fe�q��59Rgۧ;�˥���X��O��ʱ�9��`e�&�~�an�8��&�^���2 f/ �9�g��]�8�@�Q�o$��>�J��+��O;tS��g�<����8�]�WZXN�s�9��Ixi�kR�V�3A���(y�qf���2�ڃᏯ��FYj�r��ژ��V�*�WØ�]/�|�la�8f	� �WS��P����%
`%x�@	c�vq ���A�	�%�Yw���T��H�������F�i��p=ԴJ!/鼊��9�őX���*�}n�����*(_Q��U7�緮�#^�����Nfߘ�*�Dt{;�ØrV�Y}�r1I�8�O�t8�"�E�,�/N���o���}��su����Y�ċ3�&|�'����t6��1;�8�I��z �jnX�����΄�L��[���v�{C�Ë3�(k��o�$��pL/�ĕ�F.J��z��ne�w�Z��;懗q'�[u����/�xL/3�N�tQ�p ,�P��y?��ӵ�"^fT	����&��,kL/3*8O��!���B�L/s2n�8<V���Nf��`��1<�d�O=\�t+���8x���k�x���6�_\�o�̉�)���,�����]u/0:ԭ�`�ܤ��F��,f�G>T����J�jt��
�`��s��]q��a�>�>��[���8B�8P΃���#��A*�`��K�Ĉ�;��������惗���1��.9�Ğ���eE�,��������ˊ�V]����\P�ߛ�^V�cl>k��kvuk���UZ�v5x���`k>������ї�lz��x�WϢi;y�w��R8)VJ?�0G��H�E'u=ۍB81N)�+`k�!��5���,��ݑj����-��P�J�:���r$K��H!y�z��a�G�c�x�p��"Y(���o������s���z�j��n0�. �H%�qN��D^{��п�����{�1��"����o�|�
`Et��� �
&��y_���� 0�ʃ�������|�$r�ygPm|]�@��ԟ�3�x��6|k����~���wtpv������]��e�9W�~�鍮#;I5�T�
��٘i��7j��,GJ��L:��E��@� ��Ԁ�R81W?��z(�������È�"�I��l3��A�@poc4	��])�r��_֗���)>����G�c��O�t!��@G῏ą}}�3�[�`�\	R�q�R��Dמ 0�.,�O�m]���'���J}�ե���T��{�ܯA�Oi�m�X4����{Z e$��� ^��z.i R"#��g��%��'�S�0�N����!�,�<�������^�K`D���=�e@�L��u׌E�2��6�|	�A(�(���wͽY�=�4�,�x����r��=����<|o(���^��I������2�P��Y�%��P�i��;��F�W�#��g�G�����Z�kpH1Z��%�'�G��Q�U�9"����\b�zq$����>U ���yXb�zq$��_�~�9hD���� L`/��Q���`E��>�<��Q>�o�['K`D��փG�^W���(��`��y���ŉp!w��d���*�Yn1�,�Y���ŉ�$�������"���^bN}q"C��PDD���`Æ�ٔ�z�j�&�>�3{�8�M)�����.M���/1�]mR��7u`CB��E���~_V����՝��h,��D	����Z 5�������B�3��#���vĈ����7�q(��L�!���c����#����	��E�@�!����z7G��R�+�3��y��6����h�A{|��2t�.�=4�"�M�����|���n� "yF���R�Ct#�C��ʀ���E���K:s�{�+�g�ҭ��� ��~�dVD��|��Hʀ�k��v��	� V�� ���ɉ�Z�W�x#�CO�w�ɻ�4���3����ٱ��uIx#���_��{N0oo���s"� ��5_N��ޔ^m�0�G?�{9qV��K�[	fx͇	� ��g�X�3u�GJ����d�"'�g[�=�6m��q�4�@���˩p����8�Mf �:cUWP����1őm2L��^T�v��d���K��\h��
��ڼ�F��qDt��O���8��-�ͻ*�U�وm�-p�boq:hڒ0"�����\A��룭��
��j\��<}� v9c��j@��.0��s"ʀ�^�w�{��"��a�o��"Y�����^�y��:[ߋ�
��_��_I��ã� |I^�$l!�9�K�\~���B;;̷/J�,|lR�=��;�5�v=f�%q�=_��!�=D2{�+��:�f~K�*���>�}/�}�Ԯ�8���,ɺ�?��{^�`�{!޻&X�qh�́G:��^"�X�V�,l'�B�Z��A���_�6��&���x!�:��3@�{[�A��{zu��m:ۏ7�{/q��Z�P��57�{덕� �|�U�1�R���]u��tf�yi���6C�_�vު�д"�"��{�����6j���e "�2 K���� U\���~0��X�U�`�t��X��9����EE9���$������:�g� �#�E7<?>@�z�3�֕L�/��#�1B��'�������իC�x�^���^@�?����_����$����p����>�O0�K����̺W� �?o����I��%@�gbw;�����6��g;��s ���U%���.��~_�c��Z������^��~�ar�Z���~�.C���<p��-1�^-|���T��$3;�9�Xx ������^��5Vn��>�ӏ(�Ie�ُr̥W���n�Sb��d&�'	b6}y$�_�^!
�$2U���1��<[v�\���� }�Ê�H���<ĸ�K"�⠻�R;�V-ϕ��5��o܈,�����N.1q^-�����$��t�݂=�9�j���<�d�� �����dy���M��u
�J���z�$y��Rkڐv4�'����VXj��nW����Ul�1/^-�T��:��F���خV�p�����_{��NT���1ַ֩̑��;~���j����t�t��;~��Ͷ����^���W�.�;RK��T{F���f㫕�x Cḿ�a>������o��1���j�%V��y��$Э =��X睈'$;��N��t��6�+ ��Ιx��m^��$X�����x��+����:|+����z�Lx���j��I�a�+�3屪/u����`�0���9�n�����7�@��������?�����/4���F@�/�&f��ŏ����`$	x'��
s���Gͺw��)|'���=��j�#�=�8
� �;�vW��}�.���^�U���Lm��.�a	��~�c�{��q�߫hh����
i���H�t@Q��M�[>�(�8��g>��.��ɗ�ۻL$�S�L���y���ˌ��5��0\|��i]f�~�\��п�m�C̼/3j�3h�I���k�$���2'�9N�vcx!�$ �0ħ�E���I�,����eEp�F�~5���r%
̹/s��l�g�ȯ���"1W\'��<�[�y�eAX�o��;�o$>[� A	  b�}YV�{X��'0���� ��˂��&��z�vH7��_�� �ߗ�|�R��$��`601�,3�C/^'��(Q ��`��y�eA(�Q��p2�x��˂0���A��mXn���Q+4ަ_}���˂0Ǿ�%L�_�Ԥ�a��/K��EL���,zx�^`6|Y��3+U +���̅/K����!!��{ϛ�L��$߷��ޛ.��qe`y�%qnl�A #�l��0%�,��o�Iާ)9��^cj|Yҡ�e��w�:ۤb%8+�\�D����X��:	_w�U �k���
�yZ�%��e8C&�4_
�B�]�;�Sq�?�Y&�뀭Ҵ������V^1���<� �˗������e܉���E�u/O`�|)��*{茏���q�%�KA\L�,3l�)��-
o>b�|)����z��ͦ0R�-xs3�EF,����f0R�"�V�Of�~��is<��m���Ȉ�Z_&�S�ٔ�oh�1]dt>��;b��̥�Bx�d)�-�RŴz��C;�k�Vn^̶�e���"'T�X�j�?�EpB�3x:G�93f�����F�S8�ki9��`�g߈p#h�`ZA\n�G�� Q��N���l�LQx���)��_$�%��`��\���%[�gA-��b����l��ɭ�0B�f�C��EF�]f�.����܀�=K2��H���A�7��!�ڈ�4"a��u��{]!���U4~�	kU�d�M�n�@	�� [_�M�;�]�PN=5��o�N&j�6 �!̵92�>��Fg�m�Y-�?|/r*T�|�]��N���mc�39�b����Nb���^sQ�#�M�����7l~+2�/\>R��t�:��><�k�$�B8���*s��c}�i��b9.���*�i�1����Y�{� �!V@%q�����!9
���M��`��(� ����@j{x���|Q7?���M�/X�%y�Э�LM��w�c�(����Ѭ�m���@c*�(���\���T.�n`�(��WeҾI6lv+���W<��/J���~� �W�7(^L�%q�Aͧ���tV��s�E936�`�_���ϰ_��m��8��AM*O�b&���q�ɱ�+=��Dx% �qL����/��9T	��`�aJ��m����w�e�B�&�}�I�B<�Z?;W|��l�v*f�A�D&�w
�1�
�U� .�k��:,�q���xC�� /tk?��&3�ac=#�k c}���K"�kA�a>��kR�pzaRE��u�{��
Ɔѡ�x��*����e6�;���ZE�'9�Q�%qj���CL`�6���vϦ�b��W��.*��?��m:�s�4�?T�0�]T��!�)
�$rPA�bUU���2�މE�.���{l��*���G�:EF�&���G�)��"��o����D�j�"�O<&����M|�A��X{������7 �F��3u�=Ґު
bN]���]s2]�>c`Bq}�W�ŧ�Mf���ƴ�� �W��]��]��,#</ͥy����լ=?Nb��sr����ֳ���ʛvl	|'���萭��>'���Y�ʟX������s"o�	\u�y�F�+nF����뗎�v7�^K:���2Ap����/�����n�z�ߚnpT]-�:��;�<|�� ������pit{�OM�I,_C��\~&_o��i�^������O�t3S���i���������5��+��4|e�� �K�"�cRPv"<-wm���UG�i֎���XGے�R�y𔣠ޘUV���.(��Aw��� �����,��+c���7O����aW�qamtbJ�yp��ā`���[h���{�XCo�)^.��]q"(8O/����"�#&���A��3�n�4еO�.��d5�NbM�����8�i��`�|��z�8�@DgV��~0����� �R���}DA�s����;�wۉ��YN��ۖT�	�!�F����~ٙ�6F�j�=���f�g�@��%����1�!ωol�=>gnr�\�[r���s�'���E���>B����8o'��b8�ۉ����dYL�б�𢋁�i��4��`�)�2�۲^�<�Q���+I�T�xal�%��߈rx��Ϝ����1�nuI��m�H�-'�־<QA��)uK��,��@�S"�^v[wռ뺜?�¯�%A���ފ�@����,�$��G@#,}��F�8A\pk���~�xw� V�v��j�(��y��&�{s80�ĭ�v��%�Z���s��ӟ�����@      �      x������ � �      �      x������ � �      �      x������ � �      �      x������ � �      �      x������ � �      �      x������ � �      �   h   x�3�N�M-.I-R�TI�(�S0202�45PE�AHZb��e�U9�I�2�5pcUn��I(&!i�2���$d��L�*7��$C��̰+G7	Y��=... "ځ�      �   �   x�����0E��+����}BfY�10 $��VM����z{}T�(l��sy[go�����p	�sV���JC���hԅ�Nh!_ 2鎟�6H���=���e�m������7��'�~l:�7�B&�ɪ&V�dM"$P�B%v=D*\"$P�*�
'��"��Z��s�p�-��'>a�+|"$P�
���C�
�����>��O�@\�SbSB�+� =�u�      �   <  x�����\1Ec���/0�EK���gNj8p�����f:pnJ ���[�Y4~�z�ʤ��m��Tl��<�Wl]�ӗ,qZ�-j6��5���׃R��P"�^+IV�)�z�`@<�bh�s����Ԍ&^�Dy?��\*x�,�̤��A�zg���X[�х��0{%�����M�n���>2#Ν	���P�^��~�^\eL t�]�u�#��(R8���s�	<�s��YǘMW�u�����>�B|{{�=K�UgF����E�-�)җ	2M���bB��[��m�l��7x��e�g>o������G�+���8�(�      �   .   x�3�3�30�3�4202�50�50�I�QpJ,*� �=... �z	     