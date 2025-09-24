import java.io.*;
import java.util.*;

// Classe Pessoa
class Pessoa implements Serializable {
    String nome;
    String masp;
    String cpf;

    public Pessoa(String nome, String masp, String cpf) {
        this.nome = nome;
        this.masp = masp;
        this.cpf = cpf;
    }
}

// Classe Reunião
class Reuniao implements Serializable {
    Pessoa participante;
    String data;     // DD/MM/AAAA
    String horario;  // HH:MM
    String local;
    String assunto;

    public Reuniao(Pessoa participante, String data, String horario, String local, String assunto) {
        this.participante = participante;
        this.data = data;
        this.horario = horario;
        this.local = local;
        this.assunto = assunto;
    }
}

// Classe Principal
public class AgendaReunioes {
    private static final String ARQUIVO_REGISTROS = "reunioes.dat";
    private static Scanner sc = new Scanner(System.in);
    private static List<Reuniao> registros = new ArrayList<>();

    // Salvar registros em arquivo
    private static void salvarRegistros() {
        try (ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream(ARQUIVO_REGISTROS))) {
            oos.writeObject(registros);
        } catch (IOException e) {
            System.out.println("Erro ao salvar registros: " + e.getMessage());
        }
    }

    // Carregar registros do arquivo
    private static void carregarRegistros() {
        try (ObjectInputStream ois = new ObjectInputStream(new FileInputStream(ARQUIVO_REGISTROS))) {
            registros = (List<Reuniao>) ois.readObject();
            System.out.println("Dados carregados com sucesso!");
        } catch (FileNotFoundException e) {
            registros = new ArrayList<>();
        } catch (Exception e) {
            System.out.println("Erro ao carregar registros: " + e.getMessage());
        }
    }

    private static void cabecalho() {
        System.out.println("\n=== CALENDARIO DE REUNIOES ===\n");
    }

    // Login ou cadastro
    private static Pessoa realizarLogin() {
        cabecalho();
        System.out.println("--- LOGIN DO USUARIO ---");
        System.out.print("Digite seu MASP: ");
        String masp = sc.nextLine().trim();
        System.out.print("Digite seu CPF: ");
        String cpf = sc.nextLine().trim();

        for (Reuniao r : registros) {
            if (r.participante.masp.equals(masp) && r.participante.cpf.equals(cpf)) {
                System.out.println("\nBem-vindo(a) de volta, " + r.participante.nome + "!");
                return r.participante;
            }
        }

        // Cadastro novo
        System.out.println("\nUsuario nao encontrado. Realizando novo cadastro...");
        System.out.print("Digite seu Nome Completo: ");
        String nome = sc.nextLine().trim();
        Pessoa novo = new Pessoa(nome, masp, cpf);
        System.out.println("\nCadastro realizado com sucesso! Bem-vindo(a), " + nome + "!");
        return novo;
    }

    // Agendar reunião
    private static void agendarReuniao(Pessoa usuario) {
        cabecalho();
        System.out.println("AGENDAR NOVA REUNIAO PARA " + usuario.nome);

        System.out.print("Data (DD/MM/AAAA): ");
        String data = sc.nextLine().trim();
        System.out.print("Horario (HH:MM): ");
        String horario = sc.nextLine().trim();
        System.out.print("Local: ");
        String local = sc.nextLine().trim();
        System.out.print("Assunto: ");
        String assunto = sc.nextLine().trim();

        registros.add(new Reuniao(usuario, data, horario, local, assunto));
        salvarRegistros();
        System.out.println("\nReuniao agendada com sucesso!");
    }

    // Listar reuniões
    private static void listarReunioes(Pessoa usuario) {
        cabecalho();
        System.out.println("LISTA DE REUNIOES PARA " + usuario.nome);
        int cont = 0;
        for (Reuniao r : registros) {
            if (r.participante.masp.equals(usuario.masp)) {
                cont++;
                System.out.println("Reuniao " + cont + ":");
                System.out.println("   Data: " + r.data);
                System.out.println("   Horario: " + r.horario);
                System.out.println("   Local: " + r.local);
                System.out.println("   Assunto: " + r.assunto);
                System.out.println("---------------------------");
            }
        }
        if (cont == 0) System.out.println("Nenhuma reuniao agendada.");
    }

    // Cancelar reunião
    private static void cancelarReuniao(Pessoa usuario) {
        cabecalho();
        System.out.println("CANCELAR REUNIOES DE " + usuario.nome);

        List<Integer> indices = new ArrayList<>();
        int cont = 0;
        for (int i = 0; i < registros.size(); i++) {
            if (registros.get(i).participante.masp.equals(usuario.masp)) {
                cont++;
                indices.add(i);
                System.out.println(cont + ". " + registros.get(i).data + " " + registros.get(i).horario + " - " + registros.get(i).assunto);
            }
        }

        if (cont == 0) {
            System.out.println("Nenhuma reuniao encontrada.");
            return;
        }

        System.out.print("Digite o numero da reuniao a cancelar: ");
        int escolha = Integer.parseInt(sc.nextLine());
        if (escolha < 1 || escolha > cont) {
            System.out.println("Numero invalido!");
            return;
        }

        registros.remove((int) indices.get(escolha - 1));
        salvarRegistros();
        System.out.println("Reuniao cancelada com sucesso!");
    }

    // Consultar reuniões do mês
    private static void reunioesDoMes(Pessoa usuario) {
        cabecalho();
        System.out.print("Digite o mes (MM): ");
        String mes = sc.nextLine().trim();
        int cont = 0;
        for (Reuniao r : registros) {
            if (r.participante.masp.equals(usuario.masp) && r.data.substring(3, 5).equals(mes)) {
                cont++;
                System.out.println(r.data + " " + r.horario + " - " + r.assunto);
            }
        }
        if (cont == 0) System.out.println("Nenhuma reuniao encontrada neste mes.");
    }

    public static void main(String[] args) {
        carregarRegistros();
        Pessoa usuarioAtual = realizarLogin();

        boolean executando = true;
        while (executando) {
            cabecalho();
            System.out.println("Usuario: " + usuarioAtual.nome + " (MASP: " + usuarioAtual.masp + ")");
            System.out.println("Total de reunioes no sistema: " + registros.size());
            System.out.println("\n1. Agendar nova reuniao");
            System.out.println("2. Listar minhas reunioes");
            System.out.println("3. Cancelar reuniao");
            System.out.println("4. Consultar reunioes do mes");
            System.out.println("5. Sair");
            System.out.print("Escolha: ");

            int opcao = Integer.parseInt(sc.nextLine());
            switch (opcao) {
                case 1 -> agendarReuniao(usuarioAtual);
                case 2 -> listarReunioes(usuarioAtual);
                case 3 -> cancelarReuniao(usuarioAtual);
                case 4 -> reunioesDoMes(usuarioAtual);
                case 5 -> executando = false;
                default -> System.out.println("Opcao invalida!");
            }

            if (executando) {
                System.out.println("\nPressione Enter para continuar...");
                sc.nextLine();
            }
        }

        salvarRegistros();
        System.out.println("\nPrograma encerrado. Ate mais, " + usuarioAtual.nome + "!");
    }
}
