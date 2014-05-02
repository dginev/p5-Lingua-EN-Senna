use ExtUtils::MakeMaker;
our $MORE_MAKERULES = '';

# Append any additional Makefile rules added by the following.
sub MY::postamble {
  my ($self, @rules) = @_;
  return $self->MY::SUPER::postamble(@rules) . $MORE_MAKERULES; }

# Rules to build the SENNA_API interface to SENNA:
$MORE_MAKERULES .= <<'EOL';
pure_all :: SENNA_API$(OBJ_EXT)

SENNA_API$(OBJ_EXT):
	$(CCCMD) $(CCCDLFLAGS) "-I$(PERL_INC)" $(PASTHRU_DEFINE) $(DEFINE) SENNA_API/SENNA_API.c
EOL


WriteMakefile (
    NAME => 'Lingua::EN::SENNA',
    VERSION_FROM => 'lib/Lingua/EN/SENNA.pm',
    INC => "-ISENNA_API -Isenna",
    # LIBS => "-LSENNA_API -Lsenna",
    clean => {FILES=>'SENNA_API$(OBJ_EXT)'},
    OBJECT => 'SENNA_API$(OBJ_EXT) SENNA$(OBJ_EXT)',
);