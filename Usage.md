# USAGE
## When committing files to this repo, please add a thorough description in the *commit comments* and *update the README*!
*Online markdown writer link*: https://stackedit.io/app# \
Use the markdown writer- .md files are not like .txt, there's a specific format to make text **bold** and such.

### Sample README lines

`variable_name`- description/ what it does
> comments

`function_name`- description/ what it does
> comments

## Github/SourceTree stuff:
**Pull** - Updates your local files so they are the same as whatever is on this repo \
**Fetch**- Pull the repository, but do not make changes on the local machine \
**Commit** - Stage files to be pushed to the repo \
**Push-** Updates committed files to the repository, so the repository is the same as whatever is on your local machine

## Sourcetree Install Notes
I couldn't find an amazing tutorial without horrible audio, but it's very self-explanitory. \
After running the installer, a menu should pop-up with 4 categories. 
1. Install
2. Registration - Skip registering for an account
3. Install tools - Check the "Git" box (No need for Mercurial). Under ADVANCED OPTIONS, check "Configure Global Ignore"
4. Preferences - Github username on the first line, Email on the second line
AFTER this, see SourceTree Authentication section.

## Sourcetree Authentication:
Make sure you setup authentication correctly under `tools`>>`options`>>`authentication` \
**Hosting Service** : Github, \
**Protocol** : HTTPS, \
**Authentication** : OAuth (or SSH if you prefer *advanced but highly secure) \
Click `Refresh OAuth Token` and it should open a link in your browser. Verify a check mark and "Authentication OK" appears.

SEE DOCUMENTATION : (For Mac, but same process for Windows) https://confluence.atlassian.com/get-started-with-sourcetree/connect-your-bitbucket-or-github-account-847359096.html
