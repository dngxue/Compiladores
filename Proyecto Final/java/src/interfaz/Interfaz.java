package interfaz;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.InputStreamReader;
import java.io.IOException;

import java.awt.BorderLayout;
import java.awt.EventQueue;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.border.EmptyBorder;
import javax.swing.text.html.HTMLEditorKit;
import javax.swing.JLabel;
import javax.swing.JOptionPane;

import java.awt.Font;
import javax.swing.JTextArea;
import javax.swing.JTextPane;
import java.awt.Color;
import java.awt.Dimension;

import javax.swing.JButton;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;

public class Interfaz extends JFrame{
    private static final long serialVersionUID = 1L;
	private JPanel contentPane;

    private JTextArea areaCodigo;
	private JTextPane areaSalida;
	
	Color fondo = new Color(75, 90, 98);
	Color letrasBlancas = new Color(240,240,240);
	Color codigoFondo = new Color(41, 49, 52);
	
	/**
	 * Create the frame.
	 */
	public Interfaz() {
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setBounds(100, 100, 1024, 750);
		contentPane = new JPanel();
		contentPane.setBorder(new EmptyBorder(5, 5, 5, 5));
		contentPane.setBackground(fondo);

		setContentPane(contentPane);
		contentPane.setLayout(new BorderLayout(0, 0));
		
		/*******************/
		
		JPanel panelLeft = new JPanel();
		panelLeft.setOpaque(false);
		contentPane.add(panelLeft, BorderLayout.WEST);
		panelLeft.setLayout(new BorderLayout(0, 0));
		panelLeft.setBorder(new EmptyBorder(20, 20, 20, 0));
		
		JLabel lblNewLabel = new JLabel("Código");
		lblNewLabel.setForeground(letrasBlancas);
		lblNewLabel.setFont(new Font("Calibri", Font.PLAIN, 20));
		panelLeft.add(lblNewLabel, BorderLayout.NORTH);
		
		areaCodigo = new JTextArea();
		areaCodigo.setFont(new Font("Monospaced", Font.PLAIN, 15));
		areaCodigo.setTabSize(4);
		areaCodigo.setPreferredSize(new Dimension(420, 150));
		areaCodigo.setCaretColor(Color.WHITE);
		areaCodigo.setBackground(codigoFondo);
		areaCodigo.setForeground(letrasBlancas);
		panelLeft.add(areaCodigo, BorderLayout.CENTER);
		
		/*******************/
		
		JPanel panelRight = new JPanel();
		panelRight.setOpaque(false);
		contentPane.add(panelRight, BorderLayout.EAST);
		panelRight.setLayout(new BorderLayout(0, 0));
		panelRight.setBorder(new EmptyBorder(20, 0, 20, 20));
		
		JLabel lblSalida = new JLabel("Salida");
		lblSalida.setForeground(letrasBlancas);
		lblSalida.setFont(new Font("Calibri", Font.PLAIN, 20));
		panelRight.add(lblSalida, BorderLayout.NORTH);
		
		areaSalida = new JTextPane();
		areaSalida.setEditable(false);
		areaSalida.setForeground(new Color(234, 234, 234));
		areaSalida.setBackground(Color.BLACK);
		areaSalida.setFont(new Font("Consolas", Font.PLAIN, 15));
		areaSalida.setPreferredSize(new Dimension(420, 150));
		areaSalida.setEditorKit(new HTMLEditorKit());
		panelRight.add(areaSalida, BorderLayout.CENTER);
		
		/*******************/
		
		JPanel panelTop = new JPanel();
		panelTop.setOpaque(false);
		contentPane.add(panelTop, BorderLayout.NORTH);
		
		JPanel panelCompilar = new JPanel();
		panelCompilar.setOpaque(false);
		panelCompilar.setBorder(new EmptyBorder(0, 0, 0, 15));
		panelTop.add(panelCompilar, BorderLayout.CENTER);
		
		JButton btnCompilar = new JButton("Compilar");
		panelCompilar.add(btnCompilar);
		btnCompilar.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent event) {
				compilar();
			}
		});

		JPanel panelEjecutar = new JPanel();
		panelEjecutar.setOpaque(false);
		panelEjecutar.setBorder(new EmptyBorder(0, 15, 0, 0));
		panelTop.add(panelEjecutar, BorderLayout.CENTER);
		
		JButton btnEjecutar = new JButton("Ejecutar");
		btnEjecutar.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent event) {
				ejecutar();
			}
		});
		

		panelEjecutar.add(btnEjecutar);
        setVisible(true);
	}

    public void compilar() {
		escribirCodigoNuevo();
		String batPath = ".\\..\\cpp\\scripts\\compilar.bat"; // Ruta al archivo .bat
        ProcessBuilder processBuilder = new ProcessBuilder("cmd.exe", "/c", batPath);
        try {
            Process process = processBuilder.start();

            BufferedReader reader = new BufferedReader(new InputStreamReader(process.getInputStream()));
            String line;
            while ((line = reader.readLine()) != null) {
                System.out.println(line);
            }

            // Capturar la salida de error
            BufferedReader errorReader = new BufferedReader(new InputStreamReader(process.getErrorStream()));
            while ((line = errorReader.readLine()) != null) {
                System.err.println(line);
            }

            int exitCode = process.waitFor();
            System.out.println("Exited with code: " + exitCode);

        } catch (IOException | InterruptedException e) {
            e.printStackTrace();
        }

		leerSalida();
    }

    public void ejecutar() {

        escribirCodigoNuevo();
		String batPath = ".\\..\\cpp\\scripts\\ejecutar.bat"; // Ruta al archivo .bat
        ProcessBuilder processBuilder = new ProcessBuilder("cmd.exe", "/c", batPath);
        try {
            Process process = processBuilder.start();

            BufferedReader reader = new BufferedReader(new InputStreamReader(process.getInputStream()));
            String line;
            while ((line = reader.readLine()) != null) {
                System.out.println(line);
            }

            // Capturar la salida de error
            BufferedReader errorReader = new BufferedReader(new InputStreamReader(process.getErrorStream()));
            while ((line = errorReader.readLine()) != null) {
                System.err.println(line);
            }

            int exitCode = process.waitFor();
            System.out.println("Exited with code: " + exitCode);

        } catch (IOException | InterruptedException e) {
            e.printStackTrace();
        }

		leerSalida();
		
        /*
        String batPath = "C:\\Users\\maste\\eclipse-workspace\\interfaz\\cods\\correr.bat"; // Ruta al archivo .bat

        ProcessBuilder processBuilder = new ProcessBuilder("cmd.exe", "/c", batPath);
        processBuilder.directory(new java.io.File("cods")); // Directorio de trabajo

        try {
            Process process = processBuilder.start();

            BufferedReader reader = new BufferedReader(new InputStreamReader(process.getInputStream()));
            String line;
            while ((line = reader.readLine()) != null) {
                System.out.println(line);
            }

            // Capturar la salida de error
            BufferedReader errorReader = new BufferedReader(new InputStreamReader(process.getErrorStream()));
            while ((line = errorReader.readLine()) != null) {
                System.err.println(line);
            }

            int exitCode = process.waitFor();
            System.out.println("Exited with code: " + exitCode);

        } catch (IOException | InterruptedException e) {
            e.printStackTrace();
        }
        
        try (BufferedReader reader = new BufferedReader(new FileReader("cods\\out.txt"))) {
            textPane.read(reader, null);
            //JOptionPane.showMessageDialog(this, "Texto cargado desde input.txt");
        } catch (IOException e) {
            //JOptionPane.showMessageDialog(this, "Error al cargar el archivo: " + e.getMessage());
        }*/
    }

	public void escribirCodigoNuevo() {
		String codigo = areaCodigo.getText();
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(".\\..\\cpp\\archivos\\codigoNuevo.txt"))) {
            writer.write(codigo);
        } catch (IOException e) {
            JOptionPane.showMessageDialog(this, "Error al guardar el archivo: ", "Error", JOptionPane.ERROR_MESSAGE);
        }
	}

	public void leerSalida() {
		try (BufferedReader reader = new BufferedReader(new FileReader(".\\\\..\\\\cpp\\\\archivos\\\\salida.txt"))) {
			StringBuilder sb = new StringBuilder();
            String line;
            while ((line = reader.readLine()) != null) {
                sb.append(line).append("<br>");
            }
            String content = sb.toString();
			content = "<html><font color='white'>" + content + "</font></html>";
            areaSalida.setText(content);

			String original = content;
			String[] buscar = {"[91m", "[0m", "[92m", "[93m"};
			String[] reemplazar = {"<font color='red'>", "</font><font color='white'>", "<font color='green'>", "<font color='yellow'>"};

			String resultado = reemplazarSubcadenas(original, buscar, reemplazar);
			System.out.println("Texto original: " + original);
			System.out.println("Texto modificado: " + resultado);

			areaSalida.setText(resultado);
            //JOptionPane.showMessageDialog(this, "Texto cargado desde input.txt");
        } catch (IOException e) {
            //JOptionPane.showMessageDialog(this, "Error al cargar el archivo: " + e.getMessage());
        }
	}

	public static String reemplazarSubcadenas(String original, String[] buscar, String[] reemplazar) {
        if (buscar.length != reemplazar.length) {
            throw new IllegalArgumentException("Los arrays 'buscar' y 'reemplazar' deben tener la misma longitud");
        }

        String resultado = original;
        for (int i = 0; i < buscar.length; i++) {
            resultado = resultado.replace(buscar[i], reemplazar[i]);
        }
        return resultado;
    }
}
