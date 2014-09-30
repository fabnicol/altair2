
/**
 * section: xmlReader
 * synopsis: Parse and validate an XML file with an xmlReader
 * purpose: Demonstrate the use of xmlReaderForFile() to parse an XML file
 *          validating the content in the process and activating options
 *          like entities substitution, and DTD attributes defaulting.
 *          (Note that the XMLReader functions require libxml2 version later
 *          than 2.6.)
 * usage: reader2 <valid_xml_filename>
 * test: reader2 test2.xml > reader1.tmp && diff reader1.tmp $(srcdir)/reader1.res
 * author: Daniel Veillard
 * copy: see Copyright for the status of this software.
 */


#ifdef __cplusplus
extern "C" {

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

typedef struct bulletin
{
    xmlChar *nom;
    xmlChar *prénom;
    xmlChar *matricule;
    xmlChar *brut;
    xmlChar *net;
    xmlChar *net_à_payer;
    xmlChar *heures_sup;
} bulletin, *bulletinPtr;

static bulletinPtr
parseBulletin(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur)
{
    bulletinPtr ret = NULL;

    ret = (bulletinPtr) malloc(sizeof(bulletin));
    if (ret == NULL)
    {
        fprintf(stderr,"Pas assez de mémoire\n");
        return(NULL);
    }

    memset(ret, 0, sizeof(bulletin));

    cur = cur->xmlChildrenNode;

    while (cur != NULL)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"Person")) &&
                (cur->ns == ns))
            ret->name = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"Email")) &&
                (cur->ns == ns))
            ret->email = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
        cur = cur->next;
    }

    return(ret);
}

/*
 * and to print it
 */
static void
printPerson(personPtr cur)
{
    if (cur == NULL) return;
    printf("------ Person\n");
    if (cur->name) printf("	name: %s\n", cur->name);
    if (cur->email) printf("	email: %s\n", cur->email);
    if (cur->company) printf("	company: %s\n", cur->company);
    if (cur->organisation) printf("	organisation: %s\n", cur->organisation);
    if (cur->smail) printf("	smail: %s\n", cur->smail);
    if (cur->webPage) printf("	Web: %s\n", cur->webPage);
    if (cur->phone) printf("	phone: %s\n", cur->phone);
    printf("------\n");
}

/*
 * a Description for a Job
 */
typedef struct job
{
    xmlChar *projectID;
    xmlChar *application;
    xmlChar *category;
    personPtr contact;
    int nbDevelopers;
    personPtr developers[100]; /* using dynamic alloc is left as an exercise */
} job, *jobPtr;

/*
 * And the code needed to parse it
 */
static jobPtr
parseJob(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur)
{
    globalPtr ret = NULL;

    ret = (globalPtr) malloc(sizeof(global));
    if (ret == NULL)
    {
        fprintf(stderr,"Pas assez de mémoire\n");
        return(NULL);
    }
    memset(ret, 0, sizeof(global));

    /* We don't care what the top level element name is */
    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {
       if (xmlStrcmp(cur->name, (const xmlChar *) "DonneesIndiv"))
         cur = cur->next;

       cur->xmlChildrenNode;
    }

    while (cur != NULL)
    {
       if (xmlStrcmp(cur->name, (const xmlChar *) "PayeIndivMensuel"))
         cur = cur->next;

       cur->xmlChildrenNode;

        if (!xmlStrcmp(cur->name, (const xmlChar *) "Nom"))
        {
            ret->projectID = xmlGetProp(cur, (const xmlChar *) "ID");
            if (ret->projectID == NULL)
            {
                fprintf(stderr, "Project has no ID\n");
            }
        }
        if ((!xmlStrcmp(cur->name, (const xmlChar *) "Application")) &&
                (cur->ns == ns))
            ret->application =
                xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
        if ((!xmlStrcmp(cur->name, (const xmlChar *) "Category")) &&
                (cur->ns == ns))
            ret->category =
                xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
        if ((!xmlStrcmp(cur->name, (const xmlChar *) "Contact")) &&
                (cur->ns == ns))
            ret->contact = parsePerson(doc, ns, cur);
        cur = cur->next;
    }

    return(ret);
}

/*
 * and to print it
 */
static void
printJob(jobPtr cur)
{
    int i;

    if (cur == NULL) return;
    printf("=======  Job\n");
    if (cur->projectID != NULL) printf("projectID: %s\n", cur->projectID);
    if (cur->application != NULL) printf("application: %s\n", cur->application);
    if (cur->category != NULL) printf("category: %s\n", cur->category);
    if (cur->contact != NULL) printPerson(cur->contact);
    printf("%d developers\n", cur->nbDevelopers);

    for (i = 0; i < cur->nbDevelopers; i++) printPerson(cur->developers[i]);
    printf("======= \n");
}

/*
 * A pool of Gnome Jobs
 */
typedef struct gjob
{
    int nbJobs;
    jobPtr jobs[500]; /* using dynamic alloc is left as an exercise */
} gJob, *gJobPtr;


static globalPtr
parseFile(char *filename)
{
    xmlDocPtr doc;
    globalPtr ret;
    emploiPtr curjob;
    xmlNodePtr cur;

    cur = xmlDocGetRootElement(doc);
    if (cur == NULL)
    {
        fprintf(stderr,"document vide\n");
        xmlFreeDoc(doc);
        return(NULL);
    }
    ns = xmlSearchNsByHref(doc, cur,
                           (const xmlChar *) "http://www.gnome.org/some-location");
    if (ns == NULL)
    {
        fprintf(stderr,
                "document of the wrong type, GJob Namespace not found\n");
        xmlFreeDoc(doc);
        return(NULL);
    }
    if (xmlStrcmp(cur->name, (const xmlChar *) "Helping"))
    {
        fprintf(stderr,"document of the wrong type, root node != Helping");
        xmlFreeDoc(doc);
        return(NULL);
    }

    /*
     * Allocate the structure to be returned.
     */
    ret = (gJobPtr) malloc(sizeof(gJob));
    if (ret == NULL)
    {
        fprintf(stderr,"out of memory\n");
        xmlFreeDoc(doc);
        return(NULL);
    }
    memset(ret, 0, sizeof(gJob));

    /*
     * Now, walk the tree.
     */
    /* First level we expect just Jobs */
    cur = cur->xmlChildrenNode;
    while ( cur && xmlIsBlankNode ( cur ) )
    {
        cur = cur -> next;
    }
    if ( cur == 0 )
        return ( NULL );
    if ((xmlStrcmp(cur->name, (const xmlChar *) "Jobs")) || (cur->ns != ns))
    {
        fprintf(stderr,"document of the wrong type, was '%s', Jobs expected",
                cur->name);
        fprintf(stderr,"xmlDocDump follows\n");
#ifdef LIBXML_OUTPUT_ENABLED
        xmlDocDump ( stderr, doc );
        fprintf(stderr,"xmlDocDump finished\n");
#endif /* LIBXML_OUTPUT_ENABLED */
        xmlFreeDoc(doc);
        free(ret);
        return(NULL);
    }

    /* Second level is a list of Job, but be laxist */
    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar *) "Job")) &&
                (cur->ns == ns))
        {
            curjob = parseJob(doc, ns, cur);
            if (curjob != NULL)
                ret->jobs[ret->nbJobs++] = curjob;
            if (ret->nbJobs >= 500) break;
        }
        cur = cur->next;
    }

    return(ret);
}

static void
handleGjob(gJobPtr cur)
{
    int i;

    /*
     * Do whatever you want and free the structure.
     */
    printf("%d Jobs registered\n", cur->nbJobs);
    for (i = 0; i < cur->nbJobs; i++) printJob(cur->jobs[i]);
}

int main(int argc, char **argv)
{
    int i;
    gJobPtr cur;

    /* COMPAT: Do not genrate nodes for formatting spaces */
    LIBXML_TEST_VERSION
    xmlKeepBlanksDefault(0);

    for (i = 1; i < argc ; i++)
    {
        cur = parseGjobFile(argv[i]);
        if ( cur )
            handleGjob(cur);
        else
            fprintf( stderr, "Error parsing file '%s'\n", argv[i]);

    }

    /* Clean up everything else before quitting. */
    xmlCleanupParser();

    return(0);
}

#ifdef __cplusplus
}
#endif
